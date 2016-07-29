#include "echoworker.h"

namespace net{
namespace worker{

const StringRef EchoWorker::cmd_hello		= "hello\r\n";
const StringRef EchoWorker::cmd_help		= "help\r\n";
const StringRef EchoWorker::cmd_exit		= "exit\r\n";
const StringRef EchoWorker::cmd_shutdown	= "shutdown\r\n";

const StringRef EchoWorker::msg_help		=
				"Usage:\r\n"
				"   hello    - greeting\r\n"
				"   help     - this message\r\n"
				"   exit     - disconnect\r\n"
				"   shutdown - shutdown the server\r\n"
				"\r\n";

} // namespace worker
} // namespace

