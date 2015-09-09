#include <stdio.h>	// printf
#include <ctype.h>	// isspace

#include <set>		// std::vector
#include <algorithm>	// std::binary_search

#include "pair.h"

#define PROCESS_STEP	1000 * 10

static void printUsage(const char *cmd);

struct pcomp{
	bool operator()(const Pair &p1, const Pair &p2) const{
		return p1.cmp(p2) > 0;
	}
};

static void listLoad(std::set<Pair,pcomp> &list, const char *filename, bool tombstones = true);
//static void listSearch(std::set<Pair,pcomp> &list, const char *key);

static char *trim(char *s);

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

static void listLoad(std::set<Pair,pcomp> &list, const char *filename, bool tombstones){
	const size_t BUFFER_SIZE = 1024;
	static char buffer[BUFFER_SIZE];

	FILE *f = fopen(filename, "r");
	unsigned int i = 0;
	char *key;
	while( (key = fgets(buffer, BUFFER_SIZE, f)) ){
		trim(key);

		const char *val = tombstones ? nullptr : filename;

		list.insert( Pair{ key, val } );

		++i;

		if (i % ( PROCESS_STEP ) == 0){
			printf("Processed %10u records...\n", i );
		}
	}

	fclose(f);
}

static char *trim(char *s){
	char *end = s + strlen(s) - 1;
	while(end > s && isspace((unsigned char) *end))
		--end;

	*(end + 1) = 0;

	return s;
}
