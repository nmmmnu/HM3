#include <cstdio>	// printf

#include "disktable.h"


#include "db_ops_implementation.h"

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s r [file.db] [key] - load file.db, then search for the key\n",			cmd);
	printf("\t%s l [file.db] -     - load file.db, then list using iterator\n",			cmd);
	printf("\t%s L [file.db] [key] - load file.db, then list using iterator with start\n",	cmd);

	printf("\t\tFiles must be written with the extention.\n");
	printf("\t\tExample 'file.db'\n");

	printf("\n");
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 10;
	}

	// =======================

	const auto op		= argv[1];
	const auto filename	= argv[2];
	const auto key		= argv[3];

	// =======================

	DiskTable list;
	list.open(filename);
	list.printHeader();

	// =======================

	switch(op[0]){
	case 'r':	return op_filesearch(list, key);
	case 'L':	return op_list(list, key);
	case 'l':	return op_list(list);
	}

	printUsage(argv[0]);
	return 1;
}

