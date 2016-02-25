#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <fstream>

#include "duallist.h"

#include "skiplist.h"
#include "blackholelist.h"

using MemList		= SkipList;
using Table		= BlackHoleList;

using MyDualList	= DualList<SkipList,BlackHoleList>;

constexpr size_t	MEMLIST_SIZE	= 10 * 1024 * 1024;

using count_type	= MemList::count_type;

constexpr count_type	PROCESS_STEP	= 1000 * 10;


static void printUsage(const char *cmd);


static MyDualList factory(const char * /*lsmglob*/, size_t const mem_size){
	return MyDualList(
		MemList{},
		Table{},
		mem_size
	);
}

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
			printf("Processed %10lu records, %10zu bytes...\n", i, list.getMemList().getSize() );
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
	const auto lsmglob	= argv[2];

	MyDualList dl = factory(lsmglob, MEMLIST_SIZE);

	return listLoad(dl, filename);
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [file.txt] [lsm.*.bin] - load file.txt, then create / add to lsm.*.bin\n",	cmd);
	printf("\n");
}

