
#include <cstring>

namespace net{
namespace worker{

template<class CONNECTION>
WorkerStatus EchoWorker::operator()(CONNECTION &buffer){
	if (buffer.size() == 0)
		return WorkerStatus::PASS;

	// buffer.print();

	if (cmp_(cmd_shutdown, buffer)){
		return WorkerStatus::SHUTDOWN;
	}

	if (cmp_(cmd_exit, buffer)){
		return WorkerStatus::DISCONNECT;
	}


	if (cmp_(cmd_hello, buffer)){
		buffer.clear();
		buffer.push("Hello, how are you?\r\n");
		return WorkerStatus::WRITE;
	}

	if (cmp_(cmd_help, buffer)){
		buffer.clear();
		buffer.push(msg_help);
		return WorkerStatus::WRITE;
	}

	return WorkerStatus::WRITE;
}

} // namespace worker
} // namespace

