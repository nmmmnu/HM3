#ifndef _ECHO_WORKER_H
#define _ECHO_WORKER_H

#include "workerdefs.h"

#include "stringref.h"

namespace net{
namespace worker{

class EchoWorker{
public:
	template<class CONNECTION>
	WorkerStatus operator()(CONNECTION &buffer);

private:
	static const StringRef cmd_hello;
	static const StringRef cmd_help;
	static const StringRef cmd_exit;
	static const StringRef cmd_shutdown;

	static const StringRef msg_help;

private:
	template<class CONNECTION>
	static bool cmp_(const StringRef &cmd, const CONNECTION &b){
		return cmd.compare(b.data(), b.size()) == 0;
	}
};

} // namespace worker
} // namespace

// ==================================

#include "echoworker_impl.h"

#endif
