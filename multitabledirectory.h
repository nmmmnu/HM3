#ifndef _MULTI_TABLE_DIRECTORY_H
#define _MULTI_TABLE_DIRECTORY_H

#include "disktable.h"

#include "stringref.h"

#include <vector>

class MultiTableDirectory{
private:
	typedef std::vector<DiskTable>		container_type;

public:
	typedef container_type::size_type	size_type;

public:
	MultiTableDirectory() = default;

	MultiTableDirectory(const StringRef &path){
		open(path);
		//printf("%lu\n", _files.size());
	}

	bool open(const StringRef &path);

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

private:
	container_type _files;

};

#endif

