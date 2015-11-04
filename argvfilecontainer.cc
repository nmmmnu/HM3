#include "argvfilecontainer.h"

bool ArgvFileContainer::open(const int argc, const char **argv){
	if (argc < 0)
		return false;

	clear( argc );

	for (int i = 0; i < argc; ++i){
	//	printf("%d %s\n", i, argv[i]);
		pushBack( argv[i] );
	}

	return true;
}


