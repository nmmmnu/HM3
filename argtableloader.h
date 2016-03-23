#ifndef _FILE_LSM_CONTAINER_ARGV_H
#define _FILE_LSM_CONTAINER_ARGV_H

#include <vector>

#include "disktable.h"


namespace hm3{
namespace tableloader{


class ArgTableLoader{
public:
	using container_type	= std::vector<DiskTable>;

public:
	ArgTableLoader(int const argc, const char **argv) :
				_argc(argc),
				_argv(argv){
		_refresh();
	}

	const container_type &operator*() const{
		return _vector;
	}

private:
	bool _refresh();

private:
	int		_argc;
	const char	**_argv;

	container_type	_vector;
};


} // namespace tableloader
} // namespace


#endif

