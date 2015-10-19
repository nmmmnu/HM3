#ifndef _MULTI_TABLE_DIRECTORY_H
#define _MULTI_TABLE_DIRECTORY_H

#include "multitablevector.h"

class MultiTableDirectory : public MultiTableVector{
public:
	MultiTableDirectory() = default;

	MultiTableDirectory(const StringRef &path){
		open(path);
	}

	bool open(const StringRef &path);
};

#endif

