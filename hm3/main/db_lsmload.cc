#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <fstream>

#include "flushlist.h"
#include "skiplist.h"

#include "stringtokenizer.h"
#include "filereader.h"

/*
#include <vector>
#include "vectorlist.h"
#include "hashlist.h"
*/

#include "idgenerator/idgeneratordate.h"
#include "flusher/diskfileflusher.h"

//using MemList		= HashList<std::vector<SkipList> >;
using MemList		= hm3::SkipList;
using MyIDGenerator	= hm3::idgenerator::IDGeneratorDate;
using Flusher		= hm3::flusher::DiskFileFlusher<MyIDGenerator>;


using size_type	= MemList::size_type;


constexpr size_t	MEMLIST_SIZE	= 10 * 1024 * 1024;
constexpr size_type	PROCESS_STEP	= 1000 * 10;


static void printUsage(const char *cmd);


template <class LIST, class READER>
static int listLoad(LIST &list, READER &reader){
	size_t i = 0;

	while(reader){
		std::string line = reader.getLine();

		StringTokenizer tok{ line };

		const StringRef &key = tok.getNext();
		const StringRef &val = tok.getNext();

		if (! key.empty())
			list.put( { key, val } );

		++i;

		if (i % PROCESS_STEP == 0){
			printf("Processed %10zu records, In memory %10zu records, %10zu bytes...\n",
						i,
						size_t{ list.getList().getCount() },
						list.getList().getBytes() );
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

	using MyList	= hm3::FlushList<MemList,Flusher>;

	MyList mylist{
		memlist,
		Flusher{
			MyIDGenerator{},
			lsm_path,
			lsm_ext
		},
		MEMLIST_SIZE
	};

	FileReader reader{ filename };

	return listLoad(mylist, reader);
}


static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [file.txt] [lsm_path/] - load file.txt, then create / add to lsm_path/\n",	cmd);
	printf("\n");
}

