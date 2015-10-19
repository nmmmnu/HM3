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
	void clear(size_type const count = 0){
		_files.clear();

		if (count)
		_files.reserve(count);
	}
	
	bool pushBack(const StringRef &filename){
		DiskTable dt;
		dt.open(filename);
		_files.push_back( std::move(dt) );
		
		return true;
	}

private:
	container_type _files;

};


#endif

