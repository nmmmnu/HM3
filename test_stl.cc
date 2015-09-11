#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <set>		// std::vector
#include <algorithm>	// std::binary_search
#include <string>
#include <fstream>

#include "pair.h"

#define PROCESS_STEP	1000 * 10

static void printUsage(const char *cmd);

struct pcomp{
	bool operator()(const Pair &p1, const Pair &p2) const{
		return p1.cmp(p2) > 0;
	}
};

static void listLoad(std::set<Pair,pcomp> &list, const std::string &filename, bool tombstones = true);

static int op_search(std::set<Pair,pcomp> &list, const char *filename, const char *key){
	printf("Load start...\n");
	listLoad(list, filename);
	printf("Load done...\n");
	getchar();

//	printf("Search start...\n");
//	listSearch(list, key);
//	printf("Search done...\n");
//	getchar();

	return 0;
}

int main(int argc, char **argv){
	if (argc <= 2){
		printUsage(argv[0]);
		return 1;
	}

	const char *filename	= argv[1];
	const char *key		= argv[2];

	std::set<Pair,pcomp> list;

	op_search(list, filename, key);

	return 0;
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [file.txt] [key]      - load file.txt, then search for the key\n",		cmd);
	printf("\n");
}

static void listLoad(std::set<Pair,pcomp> &list, const std::string &filename, bool const tombstones){ 
	static const char *trim_ch = " \t\n";

	const std::string empty;

	std::ifstream f;
	f.open(filename);

	unsigned int i = 0;
	
	for(std::string line; getline(f, line);){
		// trim
		line.erase(line.find_last_not_of(trim_ch) + 1);

		const std::string &key = line;
		const std::string &val = tombstones ? empty : key;

		list.insert( Pair{ key, val } );

		++i;

		if (i % ( PROCESS_STEP ) == 0){
			printf("Processed %10u records, %10zu bytes...\n", i, list.size() );
		}
	}
}

