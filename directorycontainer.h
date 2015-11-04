#ifndef _DIRECTORY_CONTAINER_H
#define _DIRECTORY_CONTAINER_H

#include "vectorcontainer.h"
#include "disktable.h"

class DirectoryContainer : public VectorContainer<DiskTable>{
public:
	DirectoryContainer() = default;

	DirectoryContainer(const StringRef &path){
		open(path);
	}

	bool open(const StringRef &path);
};

#endif

