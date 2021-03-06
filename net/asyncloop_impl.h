#include "sockets.h"

#include "stringref.h"

#include "worker/workerdefs.h"

#include <unistd.h>	// read

#include <algorithm>	// find

namespace net{

template<class SELECTOR, class WORKER>
AsyncLoop<SELECTOR, WORKER>::AsyncLoop(SELECTOR &&selector, WORKER &&worker, const std::initializer_list<int> &serverFD,
			uint32_t const conf_connectionTimeout,
			size_t const conf_maxPacketSize) :
					selector_(std::move(selector)),
					worker_(std::move(worker)),
					serverFD_(serverFD),

					conf_connectionTimeout_(
						conf_connectionTimeout < CONNECTION_TIMEOUT ?
							CONNECTION_TIMEOUT :
							conf_connectionTimeout
					),
					conf_maxPacketSize_(
						conf_maxPacketSize < BUFFER_CAPACITY ?
							BUFFER_CAPACITY :
							conf_maxPacketSize
					){
	for(int const fd : serverFD_){
		socket_makeNonBlocking(fd);
		selector_.insertFD(fd);
	}
}


template<class SELECTOR, class WORKER>
AsyncLoop<SELECTOR, WORKER>::~AsyncLoop(){
	// serverFD_ will be closed if we close epoll
	for(int const fd : serverFD_)
		selector_.removeFD(fd);
}

// ===========================

template<class SELECTOR, class WORKER>
bool AsyncLoop<SELECTOR, WORKER>::process(){
	using WaitStatus = selector::WaitStatus;
	using FDStatus   = selector::FDStatus;

	keepProcessing_ = true;

	log_("poll()-ing...");
	const WaitStatus status = selector_.wait(WAIT_TIMEOUT_MS);

	if (status == WaitStatus::ERROR){
		log_("poll() error");

		return false;
	}

	if (status == WaitStatus::NONE){
		// idle loop, check for expired conn
		expireFD_();
		return true;
	}

	for(uint32_t i = 0; i < selector_.getFDStatusCount(); ++i){
		const auto &t = selector_.getFDStatus(i);

		switch(t.status){
		case FDStatus::READ:
			handleRead_( t.fd );
			break;

		case FDStatus::WRITE:
			handleWrite_( t.fd );
			break;

		case FDStatus::ERROR:
			handleDisconnect_( t.fd, DisconnectStatus::ERROR );
			break;

		case FDStatus::STOP:
			goto break2;

		case FDStatus::NONE:
		default:
			break;

		}
	}

	break2: // label for goto... ;)

	return keepProcessing_;
}

// ===========================

template<class SELECTOR, class WORKER>
bool AsyncLoop<SELECTOR, WORKER>::isServerFD_(int const fd){
	return std::find(serverFD_.cbegin(), serverFD_.cend(), fd) != serverFD_.cend();
}

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::handleRead_(int const fd){
	if ( isServerFD_(fd) ){
		while (handleConnect_(fd));
		return;
	}

	// -------------------------------------

	auto it = clients_.find(fd);

	if (it == clients_.end())
		return handleDisconnect_(fd, DisconnectStatus::PROBLEM_MAP_NOT_FOUND);

	auto &buffer = it->second;

	// -------------------------------------

	ssize_t const size = ::read(fd, inputBuffer_, BUFFER_CAPACITY);

	if (size <= 0)
		return handleSocketOps_(fd, size);

	if (buffer.size() + (size_t) size > conf_maxPacketSize_)
		return handleDisconnect_(fd, DisconnectStatus::ERROR);

	buffer.push(size, inputBuffer_);

	buffer.restartTimer();

	handleWorker_(fd, buffer);
}

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::handleWrite_(int const fd){
	if ( isServerFD_(fd) ){
		// WTF?!?
		return;
	}

	// -------------------------------------

	auto it = clients_.find(fd);

	if (it == clients_.end())
		return handleDisconnect_(fd, DisconnectStatus::PROBLEM_MAP_NOT_FOUND);

	ClientBuffer &buffer = it->second;

	// -------------------------------------

	auto available = buffer.size();

	if (available <= 0){
		// this must never happen, disconnect.
		return handleDisconnect_(fd, DisconnectStatus::PROBLEM_BUFFER_WRITE);
	}

	ssize_t const size = ::write(fd, buffer.data(), available);

	// -------------------------------------

	if (size <= 0)
		return handleSocketOps_(fd, size);

	buffer.pop(size);

	buffer.restartTimer();

	if (buffer.size() == 0){
		// process with read
		selector_.updateFD(fd, selector::FDEvent::READ);

		return;
	}
}

template<class SELECTOR, class WORKER>
bool AsyncLoop<SELECTOR, WORKER>::handleWorker_(int const fd, ClientBuffer &buffer){
//	static const char *redis_ok  = "$2\r\nOK\r\n";
//	static const char *redis_err = "-ERR Error\r\n";

	const WorkerStatus status = worker_( buffer );

	switch( status ){
	case WorkerStatus::PASS:
		return false;

	case WorkerStatus::READ:
		selector_.updateFD(fd, selector::FDEvent::READ);

		return true;

	case WorkerStatus::WRITE:
		selector_.updateFD(fd, selector::FDEvent::WRITE);

		return true;

	case WorkerStatus::DISCONNECT:
		handleDisconnect_(fd, DisconnectStatus::WORKER_NORMAL);

		return true;

	case WorkerStatus::SHUTDOWN:
	//	handleDisconnect_(fd, DisconnectStatus::WORKER_NORMAL);
		keepProcessing_ = false;

		return true;

	default:
	case WorkerStatus::DISCONNECT_ERROR:
		handleDisconnect_(fd, DisconnectStatus::WORKER_ERROR);

		return true;
	}
}

template<class SELECTOR, class WORKER>
bool AsyncLoop<SELECTOR, WORKER>::handleConnect_(int const fd){
	// fd is same as serverFD_
	int const newFD = socket_accept(fd);

	// serverFD_ is non blocking, so we do not need to check EAGAIN
	if (newFD < 0)
		return false;

	if ( insertFD_(newFD) ){
		// socket_makeNonBlocking(newFD);

		log_("Connect", newFD);
	}else{
		socket_close(newFD);

		log_("Drop connect", newFD);
	}

	return true;
}

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::handleDisconnect_(int const fd, const DisconnectStatus error){
	removeFD_(fd);

	socket_close(fd);

	switch(error){
	case DisconnectStatus::NORMAL			: return log_("Normal  Disconnect",				fd);
	case DisconnectStatus::ERROR			: return log_("Error   Disconnect",				fd);
	case DisconnectStatus::TIMEOUT			: return log_("Timeout Disconnect",				fd);

	case DisconnectStatus::WORKER_NORMAL		: return log_("Worker  Disconnect - Normal",			fd);
	case DisconnectStatus::WORKER_ERROR		: return log_("Worker  Disconnect - Error",			fd);

	case DisconnectStatus::PROBLEM_MAP_NOT_FOUND	: return log_("Problem Disconnect - FD not found",		fd);
	case DisconnectStatus::PROBLEM_BUFFER_READ	: return log_("Problem Disconnect - Read buffer full",		fd);
	case DisconnectStatus::PROBLEM_BUFFER_WRITE	: return log_("Problem Disconnect - Write buffer problem",	fd);
	};
}

// ===========================

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::handleSocketOps_(int const fd, ssize_t const size){
	if (size < 0){
		if ( socket_check_eagain() ){
			// try again
			return;
		}else{
			// error, disconnect.
			return handleDisconnect_(fd, DisconnectStatus::ERROR);
		}
	}

	if (size == 0){
		// normal, disconnect.
		return handleDisconnect_(fd, DisconnectStatus::NORMAL);
	}
}

// ===========================

template<class SELECTOR, class WORKER>
bool AsyncLoop<SELECTOR, WORKER>::insertFD_(int const fd){
	// one for server fd
	if (connectedClients_ + 1 >= selector_.maxFD() )
		return false;

	bool const result = selector_.insertFD(fd);

	if (result == false)
		return false;

	//clients_.insert(std::make_pair(fd, ClientBuffer{}));
	clients_[fd];

	++connectedClients_;

	return true;
}

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::removeFD_(int const fd){
	selector_.removeFD(fd);

	clients_.erase(fd);

	--connectedClients_;
}

template<class SELECTOR, class WORKER>
void AsyncLoop<SELECTOR, WORKER>::expireFD_(){
	for(const auto &p : clients_){
		int const fd = p.first;
		auto &c = p.second;

		if (c.expired(conf_connectionTimeout_)){
			handleDisconnect_(fd, DisconnectStatus::TIMEOUT);
			// iterator is invalid now...
			return;
		}
	}
}

} // namespace

