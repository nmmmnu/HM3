#ifndef _MULTI_TABLE_ARGV_H
#define _MULTI_TABLE_ARGV_H

#include "multitablevector.h"

class MultiTableArgv : public MultiTableVector{
public:
	MultiTableArgv() = default;

	MultiTableArgv(const int argc, const char **argv){
		load(argc, argv);
	}

	bool load(const int argc, const char **argv);
};

#endif

