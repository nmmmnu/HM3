#ifndef _MULTI_TABLE_ARGV_H
#define _MULTI_TABLE_ARGV_H

#include "multitablevector.h"

class MultiTableArgv : public MultiTableVector{
public:
	MultiTableArgv() = default;

	MultiTableArgv(const int argc, const char **argv){
		open(argc, argv);
	}

	bool open(const int argc, const char **argv);

	bool openArgv(const int argc, const char **argv){
		return open(argc - 1, & argv[1]);
	}
};

#endif

