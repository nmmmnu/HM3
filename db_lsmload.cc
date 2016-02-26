#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <fstream>

#include "flushlist.h"
#include "skiplist.h"

//#include "vectorlist.h"
//#include "hashlist.h"

#include "idgenerator.h"
#include "diskfileflush.h"



//using MemList		= HashList<VectorList<> >;
using MemList		= SkipList;
using Flusher		= DiskFileFlush<IDGeneratorDate>;
using MyList		= FlushList<MemList,Flusher>;
using count_type	= MemList::count_type;


constexpr size_t	MEMLIST_SIZE	= 10 * 1024 * 1024;
constexpr count_type	PROCESS_STEP	= 1000 * 10;



static void printUsage(const char *cmd);



template <class LIST>
static int listLoad(LIST &list, const StringRef &filename){
	constexpr const char *trim_ch = " \t\n";

	std::ifstream f;
	f.open(filename);

	count_type i = 0;

	for(std::string line; getline(f, line);){
		// trim
		line.erase(line.find_last_not_of(trim_ch) + 1);

		const StringRef key = line;
		const StringRef val = "x";

		//printf("%s\n", key.c_str());

		if (! key.empty())
			list.put( { key, val } );

		++i;

		if (i % PROCESS_STEP == 0){
			printf("Processed %10lu records, In memory %10lu records, %10zu bytes...\n",
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

	MemList memlist;

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

