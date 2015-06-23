#include <stdio.h>	// printf
#include <ctype.h>	// isspace

#include "ilist.h"

#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#define PROCESS_STEP	1000 * 10

static void listLoad(IList *list, const char *filename, bool tombstones = true);
static void listSearch(IList *list, const char *key);

static char *trim(char *s);

static void printUsage(const char *cmd);

static IList *factory(char what){
	VectorList *list;

	switch(what){
	case 'v':
		list = new VectorList();
		list->setSearchMethod(list->LINEAR_SEARCH);
		return list;

	case 'V':
		list = new VectorList();
		list->setSearchMethod(list->BINARY_SEARCH);
		return list;

	case 'l':
		return new LinkList();

	default:
	case 's':
		return new SkipList();
	}

	return list;
}

int main(int argc, char **argv){
	if (argc <= 2){
		printUsage(argv[0]);
		return 1;
	}

	const char *what = argv[1];

	const char *filename = argv[2];

	const char *key = argc > 3 ? argv[3] : NULL;

	IList *list = factory(what[0]);

	printf("Load start..\n");
	listLoad(list, filename);
	printf("Load done..\n");
	getchar();

	printf("Search start..\n");
	listSearch(list, key);
	printf("Search done..\n");
	getchar();

	delete list;

	return 0;
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [class] [file] [key] - load file, then search for the key\n", cmd);
	printf("Classes are:\n");
	printf("\t%c - %s\n", 'V', "VectorList bin search");
	printf("\t%c - %s\n", 'v', "VectorList linear search");
	printf("\t%c - %s\n", 'l', "LinkList");
	printf("\t%c - %s\n", 's', "SkipList");
	printf("\n");
}

static void listLoad(IList *list, const char *filename, bool tombstones){
	const size_t BUFFER_SIZE = 1024;
	static char buffer[BUFFER_SIZE];

	FILE *f = fopen(filename, "r");
	unsigned int i = 0;
	char *key;
	while( (key = fgets(buffer, BUFFER_SIZE, f)) ){
		trim(key);

		const char *val = tombstones ? NULL : filename;

		list->put(Pair::create(key, val));

		++i;

		if (i % ( PROCESS_STEP ) == 0){
			printf("Processed %10u records, %10zu bytes...\n", i, list->getSize() );
		}
	}

	fclose(f);
}

static void listSearch(IList *list, const char *key){
	const Pair *pair = list->get(key);

	if (pair == NULL){
		printf("Key not found...\n");
		return;
	}

	pair->print();
}

static char *trim(char *s){
	char *end = s + strlen(s) - 1;
	while(end > s && isspace((unsigned char) *end))
		--end;

	*(end + 1) = 0;

	return s;
}
