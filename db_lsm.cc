#include <cstdio>	// printf

#include "lsmtable.h"
#include "directorylsmcontainer.h"

#include "db_file_implementation.h"

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s r [file.dat] [key] - load file.dat, then search for the key\n",			cmd);
	printf("\t%s l [file.dat] -     - load file.dat, then list using iterator\n",			cmd);
	printf("\t%s L [file.dat] [key] - load file.dat, then list using iterator with start\n",	cmd);

	printf("\t\tFiles must be written without extention\n");
	printf("\t\tExample 'file.*.bin' instead of 'file.0001.bin'\n");

	printf("\n");
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 1;
	}

	// =======================

	const auto op	= argv[1];
	const auto path	= argv[2];
	const auto key	= argv[3];

	// =======================

	DirectoryLSMContainer lsmc(path);

	LSMTable<DirectoryLSMContainer> mt(lsmc);

	// =======================

	switch(op[0]){
	case 'r':	return op_filesearch(mt, key);
	case 'L':	return op_list(mt, key);
	case 'l':	return op_list(mt);
	}

	printUsage(argv[0]);
	return 1;
}
