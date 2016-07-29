#ifndef _NET_ASYNC_LOOP_H
#define _NET_ASYNC_LOOP_H

#include "selector/selectordefs.h"
#include "clientbuffer.h"

#include <map>

namespace net{

template<class SELECTOR, class WORKER, class CLIENTBUFFER = ClientBuffer<1024> >
class AsyncLoop{
public:
	constexpr static int  WAIT_TIMEOUT	=  5;
	constexpr static int  CONN_TIMEOUT	= 20;

private:
	constexpr static int  WAIT_TIMEOUT_MS	=  WAIT_TIMEOUT * 1000;

public:
	AsyncLoop(SELECTOR &&selector, WORKER &&worker, const std::initializer_list<int> &serverFD);
	~AsyncLoop();
	AsyncLoop(AsyncLoop &&other) = default;
	AsyncLoop &operator=(AsyncLoop &&other) = default;

	bool process();

private:
	enum class DisconnectStatus{
		NORMAL,
		ERROR,
		TIMEOUT,

		WORKER_NORMAL,
		WORKER_ERROR,

		PROBLEM_MAP_NOT_FOUND,
		PROBLEM_BUFFER_READ,
		PROBLEM_BUFFER_WRITE
	};

private:
	void handleRead_(int fd);
	void handleWrite_(int fd);
	bool handleConnect_(int fd);
	void handleDisconnect_(int fd, const DisconnectStatus error);
	bool handleWorker_(int fd, CLIENTBUFFER &connection);

	void handleSocketOps_(int fd, ssize_t size);

	bool isServerFD_(int fd);

private:
	bool insertFD_(int fd);
	void removeFD_(int fd);
	void expireFD_();

private:
	void log_(const char *s, int const fd = -1) const{
		if (fd < 0)
			printf("%-40s | clients: %5u |\n",         s, connectedClients_);
		else
			printf("%-40s | clients: %5u | fd: %5d\n", s, connectedClients_, fd);
	}

private:
	using ClientBufferContainer	= std::map<int, CLIENTBUFFER>;

	SELECTOR		selector_;
	WORKER			worker_;
	std::vector<int>	serverFD_;
	ClientBufferContainer	clients_;
	uint32_t		connectedClients_ = 0;
	bool			keepProcessing_ = true;
};


} // namespace

// ===========================

#include "asyncloop_impl.h"

#endif

