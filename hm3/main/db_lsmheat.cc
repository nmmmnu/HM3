#include <iostream>

#include "lsmtable.h"

#include "tableloader/directorytableloader.h"

static void printUsage(const char *cmd){
	std::cout
		<< "This program retrieve first and last element of each disktable from the LSM tree."	<< '\n'
		<< "It creates 'hotspots' into OS cache, so next searches to be fast."			<< '\n'
		<< '\n'

		<< "Usage:"	<< '\n'
		<< "\t"		<< cmd	<< "[lsm_path] - preload / heat lsm_path"	<< '\n'

		<< "\t\tPath names must be written without extention"		<< '\n'
		<< "\t\tExample 'directory/file.*'"				<< '\n'
		<< "\t\tExample 'directory/file.db'"				<< '\n'

		<< '\n';
}

int main(int argc, char **argv){
	if (argc <= 1){
		printUsage(argv[0]);
		return 1;
	}

	// =======================

	const auto path		= argv[1];

	// =======================

	using DirectoryTableLoader = hm3::tableloader::DirectoryTableLoader;
	using MyLSMTable = hm3::LSMTable<DirectoryTableLoader::container_type>;

	DirectoryTableLoader dl{ path };
	MyLSMTable mtlist(*dl);

	// =======================

	// first key
	mtlist.begin();

	const char last[] = { (char) 0xFF, '\0' };

	// ~last key
	mtlist.lowerBound( last );

	return 0;
}

