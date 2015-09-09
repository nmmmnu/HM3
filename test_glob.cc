#include "myglob.h"

#include <iostream>

static void print_usage(const char *name){
	printf("Usage:\n");
	printf("\t%s [directory] - list directory using glob()\n", name);
	printf("\t\tDo not forget about quotes around the directory\n");
}

MyGlob getGlobSimulateMove(const char *path){
	MyGlob gl;
	gl.open(path);
	return gl;
}

int main(int argc, char **argv){
	if (argc < 2){
		print_usage(argv[0]);
		return 1;
	}

	const char *path = argv[1];

	MyGlob gl = getGlobSimulateMove(path);

	for(auto s : gl.getData())
		std::cout << s << std::endl;

	return 0;
}

