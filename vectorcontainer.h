#ifndef _VECTOR_CONTAINER_H
#define _VECTOR_CONTAINER_H

#include "stringref.h"

#include <vector>

template <class TABLE>
class VectorContainer{
protected:
	typedef typename std::vector<TABLE>		container_type;

public:
	typedef typename container_type::size_type	size_type;

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
		TABLE dt;
		dt.open(filename);
		_files.push_back( std::move(dt) );

		return true;
	}

private:
	container_type _files;

};


#endif

