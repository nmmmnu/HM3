#include "lsmtable.h"
#include "tableloader/directorytableloader.h"

#include "selector/pollselector.h"
#include "protocol/redisprotocol.h"
#include "worker/keyvalueworker.h"
#include "asyncloop.h"

static void printUsage(const char *cmd);

// ==================================

template<class LIST>
class DBAdapter{
private:
	constexpr static size_t MAX_RESULTS = 50;

public:
	DBAdapter(LIST &list) : list_(list){}

	std::string get(const StringRef &key) const{
		const auto &p = list_.get(key);
		std::string s = p.getVal();
		std::cout << s << std::endl;
		return s;
	}

	std::vector<std::string> getall(const StringRef &key) const{
		std::vector<std::string> result;

		result.reserve(MAX_RESULTS * 2);

		const auto bit = list_.getIterator(key);
		const auto eit = list_.end();

		size_t c = 0;
		for(auto it = bit; it != eit; ++it){
			result.push_back(it->getKey());
			result.push_back(es_(it->getVal()));

			if (++c >= MAX_RESULTS)
				break;
		}

		for(auto x : result)
			std::cout << x << std::endl;

		return result;
	}

private:
	static std::string es_(const StringRef &data){
		return data.empty() ? "0" : data;
	}

private:
	LIST &list_;
};

// ==================================

int main(int argc, char **argv){
	constexpr const char	*HOSTNAME	= "localhost.not.used.yet";
	constexpr int		PORT		= 2000;

	constexpr size_t	BUFFER		= 1024 * 16;

	// ==================================

	if (argc <= 1){
		printUsage(argv[0]);
		return 1;
	}

	const auto path = argv[1];

	// ==================================

	using DirectoryTableLoader	= hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable		= hm3::LSMTable<DirectoryTableLoader::container_type>;

	DirectoryTableLoader dl{ path };
	MyLSMTable list(*dl);

	// ==================================

	using MyAdapter = DBAdapter<MyLSMTable>;
	MyAdapter adapter(list);

	// ==================================

	using MySelector		= net::selector::PollSelector;
	using MyProtocol		= net::protocol::RedisProtocol;
	using MyWorker			= net::worker::KeyValueWorker<MyProtocol, MyAdapter>;

	int const fd1 = net::socket_create(net::socket_tcp, HOSTNAME, PORT);

	net::AsyncLoop<MySelector, MyWorker, net::ClientBuffer<BUFFER> > loop( MySelector{ 4 }, MyWorker{ &adapter }, { fd1 } );

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

