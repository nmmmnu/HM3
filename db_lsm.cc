#include <cstdio>	// printf

#include "lsmtable.h"

using Pair	= hm3::Pair;

#include "db_helper_implementation.h"

#include "directorytableloader.h"

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s r [lsm_path] [key] - load lsm_path, then search for the key\n",			cmd);
	printf("\t%s l [lsm_path] -     - load lsm_path, then list using iterator\n",			cmd);
	printf("\t%s L [lsm_path] [key] - load lsm_path, then list using iterator with start\n",	cmd);

	printf("\t\tPath names must be written without extention\n");
	printf("\t\tExample 'directory/file.*' instead of 'directory/file.*.db'\n");

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

	using DirectoryTableLoader = hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable = hm3::LSMTable<DirectoryTableLoader::container_type>;

	DirectoryTableLoader dl{ path };
	MyLSMTable mtlist(*dl);

	// =======================

	switch(op[0]){
	case 'r':	return op_filesearch(mtlist, key);
	case 'L':	return op_list(mtlist, key);
	case 'l':	return op_list(mtlist);
	}

	printUsage(argv[0]);
	return 1;
}

