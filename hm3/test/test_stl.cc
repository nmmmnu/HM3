#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <set>		// std::vector
#include <algorithm>	// std::binary_search
#include <string>
#include <fstream>

#include "pair.h"

#include "filereader.h"


constexpr unsigned int PROCESS_STEP = 1000 * 10;

using Pair = hm3::Pair;

static void printUsage(const char *cmd);

struct pcomp{
	bool operator()(const Pair &p1, const Pair &p2) const{
		return p1.cmp(p2) > 0;
	}
};

using MySet = std::set<Pair,pcomp>;

template<class LIST, class READER>
void listLoad(LIST &list, READER &reader, bool tombstones = true);

template<class LIST>
int op_search(LIST &list, const char *filename, const char *key){
	FileReader reader{ filename };

	printf("Load start...\n");
	listLoad(list, reader);
	printf("Load done...\n");
	getchar();

#if 0
	printf("Search start...\n");
	listSearch(list, key);
	printf("Search done...\n");
	getchar();
#endif

	return 0;
}

int main(int argc, char **argv){
	if (argc <= 2){
		printUsage(argv[0]);
		return 1;
	}

	const char *filename	= argv[1];
	const char *key		= argv[2];

	MySet list;

	op_search(list, filename, key);

	return 0;
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [file.txt] [key]      - load file.txt, then search for the key\n",		cmd);
	printf("\n");
}

template<class LIST, class READER>
void listLoad(LIST &list, READER &reader, bool const tombstones){
	const std::string empty;

	unsigned int i = 0;

	while(reader){
		std::string line = reader.getLine();

		const std::string &key = line;
		const std::string &val = tombstones ? empty : key;

		list.insert( Pair{ key, val } );

		++i;

		if (i % PROCESS_STEP == 0){
			printf("Processed %10u records, %10zu bytes...\n", i, list.size() );
		}
	}
}

