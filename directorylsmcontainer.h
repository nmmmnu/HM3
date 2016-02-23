#ifndef _DIRECTORY_LSM_CONTAINER_H
#define _DIRECTORY_LSM_CONTAINER_H

#include <string>

#include "lsmcontainer.h"

class DirectoryLSMContainer{
public:
	using container_type	= LSMContainer::container_type;

public:
	DirectoryLSMContainer(const StringRef &path) :
				_path(path){
		refresh();
	}

public:
	container_type &get(){
		return _vector;
	}

	bool refresh();

private:
	std::string	_path;
	container_type	_vector;
};

#endif

