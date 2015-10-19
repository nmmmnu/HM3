#include "multitableargv.h"

bool MultiTableArgv::load(const int argc, const char **argv){
	_files.clear();
	
	if (argc <= 0)
		return false;

	_files.reserve( argc );

	for (int i = 0; i < argc; ++i){
		const char *filename = argv[i];
		DiskTable dt;
		dt.open(filename);
		_files.push_back( std::move(dt) );
	}

	return true;
}


