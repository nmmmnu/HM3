#ifndef _ARGV_FILE_CONTAINER_H
#define _ARGV_FILE_CONTAINER_H

#include "vectorcontainer.h"
#include "disktable.h"

class ArgvFileContainer : public VectorContainer<DiskTable>{
public:
	ArgvFileContainer() = default;

	ArgvFileContainer(const int argc, const char **argv){
		open(argc, argv);
	}

	bool open(const int argc, const char **argv);

	bool openArgv(const int argc, const char **argv){
		return open(argc - 1, & argv[1]);
	}
};

#endif

