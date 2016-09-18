#include <iostream>

#include "disktable.h"

#include "db_helper_impl.h"

static void printUsage(const char *cmd){
	std::cout
		<< "Usage:"	<< std::endl
		<< "\t"		<< cmd	<< " r [file.db] [key] - load file.db, then search for the key"	<< std::endl
		<< "\t"		<< cmd	<< " l [file.db] -     - load file.db, then list using iterator"	<< std::endl
		<< "\t"		<< cmd	<< " l [file.db] [key] - load file.db, then list using iterator"	<< std::endl

		<< "\t\tPath names must be written without extention"		<< std::endl
		<< "\t\tExample 'directory/file.db'"				<< std::endl

		<< std::endl;
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 10;
	}

	// =======================

	const auto op		= argv[1];
	const auto filename	= argv[2];

	size_t const keyCount	= (size_t) argc - 3;
	char **keys		= & argv[3];

	// =======================

	using DiskTable = hm3::DiskTable;

	DiskTable list;
	list.open(filename);
	list.printHeader();

	// =======================

	switch(op[0]){
	case 'r':	return op_filesearch(list, keyCount, keys);
	case 'l':	return op_list(list, keyCount, keys);
	}

	printUsage(argv[0]);
	return 1;
}

