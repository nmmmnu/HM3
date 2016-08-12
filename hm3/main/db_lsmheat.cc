#include <iostream>

#include "lsmtable.h"

#include "tableloader/directorytableloader.h"

static void printUsage(const char *cmd){
	std::cout
		<< "This program retrieve first and last element of each disktable from the LSM tree."	<< std::endl
		<< "It creates 'hotspots' into OS cache, so next searches to be fast."			<< std::endl
		<< std::endl

		<< "Usage:"	<< std::endl
		<< "\t"		<< cmd	<< "[lsm_path] - preload / heat lsm_path"	<< std::endl

		<< "\t\tPath names must be written without extention"		<< std::endl
		<< "\t\tExample 'directory/file.*'"				<< std::endl
		<< "\t\tExample 'directory/file.db'"				<< std::endl

		<< std::endl;
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

