#include <stdio.h>	// printf
#include <ctype.h>	// isspace

#include "diskmanager.h"

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
		list->setLookupMethod(VectorList::LINEAR_SEARCH);
		return list;

	case 'V':
		list = new VectorList();
		list->setLookupMethod(VectorList::BINARY_SEARCH);
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
	if (argc <= 4){
		printUsage(argv[0]);
		return 1;
	}

	const char *op		= argv[1];
	const char *what	= argv[2];
	const char *filename	= argv[3];
	const char *key		= argv[4];
	const char *filename2	= argv[4];

	IList *list = factory(what[0]);

	switch(op[0]){
	case 's':
		printf("Load start..\n");
		listLoad(list, filename);
		printf("Load done..\n");
		getchar();

		printf("Search start..\n");
		listSearch(list, key);
		printf("Search done..\n");
		getchar();

		break;

	case 'w':
		printf("Load start..\n");
		listLoad(list, filename);
		printf("Load done..\n");
		getchar();

		printf("Search start..\n");
		DiskManager::create(filename2, list);
		printf("Search done..\n");
		getchar();

		break;
	}


	delete list;

	return 0;
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s [op] [class] [file] [key]  - load file.txt, then search for the key\n", cmd);
	printf("\t%s [op] [class] [file] [file] - load file.txt, then create file.dat\n", cmd);
	printf("Operations are:\n");
	printf("\t%c - %s\n", 's', "Search for key");
	printf("\t%c - %s\n", 'w', "Create a file");
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
