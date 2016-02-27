#ifndef _FILE_LSM_CONTAINER_ARGV_H
#define _FILE_LSM_CONTAINER_ARGV_H

#include <vector>

#include "disktable.h"

class ArgTableLoader{
public:
	using container_type	= std::vector<DiskTable>;

public:
	ArgTableLoader(int const argc, const char **argv) :
				_vector(_load(argc, argv)){}

	const container_type &operator*() const{
		return _vector;
	}

private:
	static container_type _load(int argc, const char **argv);

private:
	container_type _vector;
};

#endif

