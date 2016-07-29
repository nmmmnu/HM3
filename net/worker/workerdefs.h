#ifndef _WORKER_DEFS_H
#define _WORKER_DEFS_H

namespace net{

	enum class WorkerStatus{
		PASS,
		READ,
		WRITE,
		DISCONNECT,
		DISCONNECT_ERROR,
		SHUTDOWN
	};

} // namespace

#endif
