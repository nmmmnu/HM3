#include "lsmtable.h"
#include "tableloader/directorytableloader.h"

#include "selector/pollselector.h"
#include "protocol/redisprotocol.h"
#include "worker/keyvalueworker.h"
#include "asyncloop.h"

#include "dbadapter.h"

static void printUsage(const char *cmd);

int main(int argc, char **argv){
	constexpr const char	*HOSTNAME		= "localhost.not.used.yet";
	constexpr int		PORT			= 2000;

	constexpr size_t	MAX_CLIENTS		= 1024;
	constexpr uint32_t	CONNECTION_TIMEOUT	= 30;
	constexpr size_t	MAX_PACKET_SIZE		= 1024 * 64;

	// ----------------------------------

	if (argc <= 1){
		printUsage(argv[0]);
		return 1;
	}

	const auto path = argv[1];

	// ----------------------------------

	using MyTableLoader	= hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable	= hm3::LSMTable<MyTableLoader::container_type>;

	MyTableLoader dl{ path };
	MyLSMTable list(*dl);

	// ----------------------------------

	using MyAdapter = DBAdapter<MyLSMTable>;
	MyAdapter adapter(list);

	// ----------------------------------

	using MySelector	= net::selector::PollSelector;
	using MyProtocol	= net::protocol::RedisProtocol;
	using MyWorker		= net::worker::KeyValueWorker<MyProtocol, MyAdapter>;

	using MyLoop		= net::AsyncLoop<MySelector, MyWorker>;

	int const fd1 = net::socket_create(net::socket_tcp, HOSTNAME, PORT);

	MyLoop loop( MySelector{ MAX_CLIENTS }, MyWorker{ &adapter }, { fd1 },
							CONNECTION_TIMEOUT, MAX_PACKET_SIZE);

	while(loop.process());
}

// ==================================

static void printUsage(const char *cmd){
	std::cout
		<< "Usage:"	<< std::endl
		<< "\t"		<< cmd	<< " [lsm_path] - start server"		<< std::endl

		<< "\t\tPath names must be written without extention"		<< std::endl
		<< "\t\tExample 'directory/file.*'"				<< std::endl

		<< std::endl;
}
