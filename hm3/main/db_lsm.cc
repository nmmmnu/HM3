#include <iostream>

#include "lsmtable.h"

#include "db_helper_impl.h"

#include "tableloader/directorytableloader.h"

static void printUsage(const char *cmd){
	std::cout
		<< "Usage:"	<< '\n'
		<< "\t"		<< cmd	<< " r [lsm_path] [key] - load lsm_path, then search for the key"	<< '\n'
		<< "\t"		<< cmd	<< " l [lsm_path] -     - load lsm_path, then list using iterator"	<< '\n'
		<< "\t"		<< cmd	<< " l [lsm_path] [key] - load lsm_path, then list using iterator"	<< '\n'

		<< "\t\tPath names must be written without extention"		<< '\n'
		<< "\t\tExample 'directory/file.*'"				<< '\n'

		<< '\n';
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 1;
	}

	// =======================

	const auto op		= argv[1];
	const auto path		= argv[2];

	size_t const keyCount	= (size_t) argc - 3;
	char **keys		= & argv[3];

	// =======================

	using DirectoryTableLoader = hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable = hm3::LSMTable<DirectoryTableLoader::container_type>;

	DirectoryTableLoader dl{ path };
	MyLSMTable mtlist(*dl);

	// =======================

	switch(op[0]){
	case 'r':	return op_filesearch(mtlist, keyCount, keys);
	case 'l':	return op_list(mtlist, keyCount, keys);
	}

	printUsage(argv[0]);
	return 1;
}

