#ifndef _MULTI_TABLE_VECTOR_H
#define _MULTI_TABLE_VECTOR_H

#include "disktable.h"

#include <vector>

class MultiTableVector{
protected:
	typedef std::vector<DiskTable>		container_type;

public:
	typedef container_type::size_type	size_type;

public:
	auto size() const{
		return _files.size();
	}

	const auto &operator[](size_type const id) const{
		return _files[id];
	}

	auto begin() const{
		return _files.begin();
	}

	auto end() const{
		return _files.end();
	}

protected:
	container_type _files;

};


#endif

