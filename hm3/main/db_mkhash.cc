#include "disktable.h"

#include "hashindex/hashindexbuilder.h"



#include <unistd.h>	// access

#include <iostream>


constexpr bool CHECK_FILE_EXISTS = false;


static void printUsage(const char *name);
static bool fileExists(const StringRef& name);

int main(int argc, char **argv){
	if (argc <= 1 + 1){
		printUsage(argv[0]);
		return 1;
	}

	const char *input_file	= argv[1];
	const char *output_file	= argv[2];

	if (CHECK_FILE_EXISTS && fileExists(output_file)){
		std::cout << "File " << output_file << " exists. Please remove it and try again." << '\n';
		return 2;
	}

	using DiskTable = hm3::DiskTable;

	DiskTable list;
	list.open(input_file);

	using HashIndexBuilder = hm3::hashindex::HashIndexBuilder;

	HashIndexBuilder htb;
	bool const result = htb.createFromList(output_file, list);

	return result ? 0 : 100;
}



static bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

static void printUsage(const char *name){
	std::cout
		<< "Usage:"						<< '\n'
		<< name << " [file.db] [hash_file]"			<< '\n'
		<< "\t\tPath names must be written without extention"	<< '\n'
		<< "\t\tExample 'file.db'"				<< '\n'
		<< "\t\tDo not forget that usually btree_file must have same name as [file.db]"	<< '\n'
	;
}

