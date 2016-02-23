#ifndef _FILE_LSM_CONTAINER_ARGV_H
#define _FILE_LSM_CONTAINER_ARGV_H

#include "lsmcontainer.h"

class ArgLSMContainer{
public:
	using container_type = LSMContainer::container_type;

public:
	ArgLSMContainer(int const argc, const char **argv) :
				_vector(_load(argc, argv)){}

public:
	container_type &get(){
		return _vector;
	}

	bool refresh(){
		return false;
	}

private:
	container_type _load(int const argc, const char **argv);

private:
	container_type _vector;
};

#endif

