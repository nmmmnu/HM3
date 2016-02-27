#ifndef _DIRECTORY_LSM_CONTAINER_H
#define _DIRECTORY_LSM_CONTAINER_H

#include <string>
#include <vector>

#include "disktable.h"

class DirectoryTableLoader{
public:
	using container_type	= std::vector<DiskTable>;

public:
	DirectoryTableLoader(const StringRef &path) : _path(path){
		refresh();
	}

	const container_type &operator*() const{
		return _vector;
	}

	bool refresh();

private:
	std::string	_path;
	container_type	_vector;
};

#endif

