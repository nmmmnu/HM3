#include "multitable.h"
#include "multitableargv.h"
#include "diskfile.h"

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("\t%s m [file1] [file2] [file...] - merge files, keep non valid\n", 		name);
	printf("\t%s M [file1] [file2] [file...] - merge files, remove non valid\n", 		name);
	printf("\t\tDo not forget you need at least two files\n");
}



int main(int argc, char **argv){
	if (argc <= 1 + 1 + 1){
		printUsage(argv[0]);
		return 1;
	}

//	const bool keep		= argv[1][0] == 'm' ? true : false;
	const char **path	= (const char **) &argv[2];
	const int  pathc	= argc - 1 - 1;

	MultiTableArgv container;
	container.open(pathc, path);

	MultiTable<MultiTableArgv> mtlist(container);

	DiskFile::create(mtlist, "test.bin");
}
