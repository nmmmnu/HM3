#include "myglob.h"

#include <stdio.h>

static void print_usage(const char *name){
	printf("Usage:\n");
	printf("\t%s [directory] - list directory using glob()\n", name);
	printf("\t\tDo not forget about quotes around the directory\n");
}

int main(int argc, char **argv){
	if (argc < 2){
		print_usage(argv[0]);
		return 1;
	}

	const char *path = argv[1];

	std::vector<std::string> files;

	MyGlob::glob(path, files);

	for(size_t i = 0; i < files.size(); ++i)
		printf("%5zu %s\n", i, files[i].c_str());

	printf("Total %zu files...\n",  files.size());

	return 0;
}

