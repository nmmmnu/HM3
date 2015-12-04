#include "lsmtable.h"
#include "filecontainerargv.h"
#include "diskfile.h"

#include <vector>
#include <unistd.h>	// access

static void printUsage(const char *name){
	const char *txt =
		"\t%s %c output_file [file1] [file2] [fileN...]"
		"- merge files, %-6s non valid, %-6s tombstones\n";

	printf("Usage:\n");
	printf(txt, 	name, '-', "keep",   "keep");
	printf(txt, 	name, 'v', "remove", "keep");
	printf(txt, 	name, 't', "remove", "remove");
	printf("\t\tDo not forget you usually need two output files\n");
}

inline bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

int main(int argc, char **argv){
	if (argc <= 1 + 1 + 1){
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

	DiskFile df = DiskFile(output);

	df.createFromList(mtlist, keepInvalid, keepTombstones);
}


