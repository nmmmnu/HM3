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
	const     size_t	MAX_PACKET_SIZE		= hm3::Pair::MAX_SIZE * 2;

	// ----------------------------------

	if (argc <= 1){
		printUsage(argv[0]);
		return 1;
	}

	const auto path = argv[1];

	// ----------------------------------

	using MyTableLoader	= hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable	= hm3::LSMTable<MyTableLoader::container_type>;

	MyTableLoader loader{ path };
	MyLSMTable list(*loader);

	// ----------------------------------

	using MyAdapter = DBAdapter<MyLSMTable, MyTableLoader>;
	MyAdapter adapter(list, loader);

	// ----------------------------------

	using MySelector	= net::selector::PollSelector;
	using MyProtocol	= net::protocol::RedisProtocol;
	using MyWorker		= net::worker::KeyValueWorker<MyProtocol, MyAdapter>;

	using MyLoop		= net::AsyncLoop<MySelector, MyWorker>;

	int const fd1 = net::socket_create(net::SOCKET_TCP{}, HOSTNAME, PORT);

	MyLoop loop( MySelector{ MAX_CLIENTS }, MyWorker{ adapter }, { fd1 },
							CONNECTION_TIMEOUT, MAX_PACKET_SIZE);

	while(loop.process());
}

// ==================================

static void printUsage(const char *cmd){
	std::cout
		<< "Usage:"	<< '\n'
		<< "\t"		<< cmd	<< " [lsm_path] - start server"		<< '\n'

		<< "\t\tPath names must be written without extention"		<< '\n'
		<< "\t\tExample 'directory/file.*'"				<< '\n'

		<< '\n';
}

