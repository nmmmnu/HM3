
#include <cstring>

#include "protocol/protocoldefs.h"


namespace net{
namespace worker{


template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::operator()(CONNECTION &buffer){
	if (buffer.size() == 0)
		return WorkerStatus::PASS;

	using Status = net::protocol::ProtocolStatus;

	const Status status = protocol_( StringRef{ buffer.data(), buffer.size() } );

	switch(status){
		case Status::BUFFER_NOT_READ :
			return WorkerStatus::PASS;

		case Status::OK :
			return process_request_(buffer);

		default: // for GCC warning...
		case Status::ERROR :
			return sendResponseError_(buffer, "Internal Error");
	}

	// must not get here
	//return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::process_request_(CONNECTION &buffer){
	protocol_.print();

	const auto &p = protocol_.getParams();
	const auto &cmd = p[0];

	if (cmd == SHUTDOWN || cmd == SHUTDOWN2){
		return WorkerStatus::SHUTDOWN;
	}

	if (cmd == EXIT || cmd == EXIT2){
		return WorkerStatus::DISCONNECT;
	}

	if (cmd == RELOAD || cmd == RELOAD2){
		return sendResponseString_(buffer, "OK");
	}

	if (cmd == INFO || cmd == INFO2){
		if (p.size() != 1)
			return sendResponseError_(buffer, "Bad request");

		return sendResponseString_(buffer, db_.info() );
	}

	if (cmd == GET || cmd == GET2){
		if (p.size() != 2)
			return sendResponseError_(buffer, "Bad request");

		const auto &key = p[1];

		return sendResponseString_(buffer, db_.get(key));
	}

	if (cmd == GETALL || cmd == GETALL2){
		if (p.size() != 2)
			return sendResponseError_(buffer, "Bad request");

		const auto &key = p[1];
		(void) key;

		return sendResponseMulti_(buffer, db_.getall(key) );
	}

	return sendResponseError_(buffer, "Not Implemented");
}

// ====================================

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseError_(CONNECTION &buffer, const StringRef &msg){
	buffer.clear();
	protocol_.response_error(buffer, msg);
	return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseString_(CONNECTION &buffer, const StringRef &msg){
	buffer.clear();
	protocol_.response_string(buffer, msg);
	return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION, class CONTAINER>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseMulti_(CONNECTION &buffer, const CONTAINER &msg){
	buffer.clear();
	protocol_.response_strings(buffer, msg);
	return WorkerStatus::WRITE;
}

} // namespace worker
} // namespace

