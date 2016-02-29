#include <cstdio>	// printf
#include <inttypes.h>	// PRIu64
#include <ctype.h>	// isspace

#include <fstream>

#include "flushlist.h"
#include "skiplist.h"

//#include <vector>
//#include "vectorlist.h"
//#include "hashlist.h"

#include "idgenerator.h"
#include "diskfileflush.h"



//using MemList		= HashList<std::vector<SkipList> >;
using MemList		= SkipList;
using Flusher		= DiskFileFlush<IDGeneratorDate>;
using MyList		= FlushList<MemList,Flusher>;
using count_type	= MemList::count_type;


constexpr size_t	MEMLIST_SIZE	= 10 * 1024 * 1024;
constexpr count_type	PROCESS_STEP	= 1000 * 10;



static void printUsage(const char *cmd);

static std::pair<StringRef, StringRef> explode(const std::string &line);



template <class LIST>
static int listLoad(LIST &list, const StringRef &filename){

	std::ifstream f;
	f.open(filename);

	count_type i = 0;

	for(std::string line; getline(f, line);){
		//trim(line);

		const auto kvp = explode(line);

		const StringRef &key = kvp.first;
		const StringRef &val = kvp.second;

		printf("%s(del)%s(end\n", key.c_str(), val.c_str());

		if (! key.empty())
			list.put( { key, val } );

		++i;

		if (i % PROCESS_STEP == 0){
			printf("Processed %10" PRIu64 " records, In memory %10" PRIu64 " records, %10zu bytes...\n",
						i,
						list.getList().getCount(),
						list.getList().getSize() );
		}
	}

	return 0;
}



int main(int argc, char **argv){
	if (argc <= 2){
		printUsage(argv[0]);
		return 1;
	}

	const auto filename	= argv[1];
	const auto lsm_path	= argv[2];
	const auto lsm_ext	= ".db";

	MemList memlist{ 32 };

	MyList mylist{
		memlist,
		Flusher{
			IDGeneratorDate{},
			lsm_path,
			lsm_ext
		},
		MEMLIST_SIZE
	};

	return listLoad(mylist, filename);
}



static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [file.txt] [lsm_path/] - load file.txt, then create / add to lsm_path/\n",	cmd);
	printf("\n");
}

/*
static std::string &trim(std::string &line){
	constexpr const char *trim_ch = " \t\r\n";

	line.erase(line.find_last_not_of(trim_ch) + 1);

	return line;
}
*/

std::pair<StringRef, StringRef> explode(const std::string &line){
	constexpr const char *delimiters	= "\t";
	constexpr const char *trim_ch		= "\t\r\n ";

	size_t const tr = line.find_last_not_of(trim_ch) + 1;
	size_t const ix = line.find_first_of(delimiters);

	if (ix >=  tr){ // std::string::npos
		return std::make_pair(
			StringRef( line.c_str() ),
			StringRef()
		);
	}

	return std::make_pair(
			StringRef( &line.c_str()[0     ], ix - 1),
			StringRef( &line.c_str()[ix + 1]        )
	);
}

