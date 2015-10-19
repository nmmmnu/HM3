#include "multitableargv.h"

bool MultiTableArgv::load(const int argc, const char **argv){
	if (argc < 0)
		return false;

	clear( argc );

	for (int i = 0; i < argc; ++i){
		pushBack( argv[i] );
	}

	return true;
}


