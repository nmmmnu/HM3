#include <stdio.h>	// printf
#include <ctype.h>	// isspace

#include "dirtable.h"

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("\t%s s [directory] [key] - open directory then search for the key\n", 		name);
	printf("\t%s l [directory] -     - open directory then List dir using iterator\n",	name);
	printf("\t%s L [directory] [key] - open directory then List dir using iterator\n",	name);
	printf("\t\tDo not forget about quotes around the directory\n");
}

void find(IROList &list, const char *key){
	const Pair *pair = list.get(key);

	if (! pair){
		printf("Key '%s' not found...\n", key);
		return;
	}

	pair->print();
}

void listing(IROList &list, const char *key = nullptr, size_t count = 100){
	for(const Pair *pair = list.first(key); pair; pair = list.next()){
		pair->print();

		if (--count == 0)
			break;
	}
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 1;
	}

	const char *op		= argv[1];
	const char *path	= argv[2];
	const char *key		= argv[3];

	DirTable dt;
	dt.open(path);

	switch(op[0]){
		case 's':
			find(dt, key);
			break;

		case 'l':
			listing(dt);
			break;

		case 'L':
			listing(dt, key);
			break;
	}

	return 0;
}
