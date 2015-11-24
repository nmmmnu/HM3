#include "lsmtable.h"
#include "filecontainerargv.h"
#include "diskfile.h"

#include <vector>
#include <unistd.h>	// access

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("\t%s - output_file [file1] [file2] [file...] - merge files, keep   non valid, keep   tombstones\n", 	name);
	printf("\t%s v output_file [file1] [file2] [file...] - merge files, remove non valid, keep   tombstones\n", 	name);
	printf("\t%s t output_file [file1] [file2] [file...] - merge files, remove non valid, remove tombstones\n", 	name);
	printf("\t\tDo not forget you need at least two files\n");
}

inline bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

int main(int argc, char **argv){
	if (argc <= 1 + 1 + 2){
		printUsage(argv[0]);
		return 1;
	}

	bool keepInvalid	= true;
	bool keepTombstones	= true;

	switch(argv[1][0]){
	case 'v':
		keepInvalid	= false;
		keepTombstones	= true;
		break;

	case 't':
		keepInvalid	= false;
		keepTombstones	= false;
		break;
	}

	const char *output	= argv[2];

	const char **path	= (const char **) &argv[3];
	const int  pathc	= argc - 3;

	if (fileExists(output)){
		printf("File %s exists. Please remove it and try again.\n", output);
		return 2;
	}

	typedef std::vector<DiskTable> MyDTVector;

	MyDTVector container;
	FileListContainer::load(container, pathc, path);

	LSMTable<MyDTVector> mtlist(container);

	DiskFile::create(mtlist, output, keepInvalid, keepTombstones);
}


