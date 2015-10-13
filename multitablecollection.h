#ifndef _MULTI_TABLE_COLLECTION_H
#define _MULTI_TABLE_COLLECTION_H

#include "disktable.h"

#include "stringref.h"

#include <vector>

class MultiTableCollectionDir{
public:
	typedef std::vector<DiskTable>	dt_vector;
	typedef dt_vector::size_type	size_type;

public:
	MultiTableCollectionDir() = default;

	explicit MultiTableCollectionDir(const StringRef &path){
		open(path);
	}

	bool open(const StringRef &path);
	void close();

private:
	const DiskTable & operator[](size_type index) const;

	size_type size() const{
		return _files.size();
	}

private:
	dt_vector _files;

};

// ==============================

inline const DiskTable & MultiTableCollectionDir::operator[](size_type const index) const{
	auto const count = size();
	if (count == 0){
		std::logic_error exception("collection is empty");
		throw exception;
	}

	if (index >= count){
		std::logic_error exception("collection index out of bounds");
		throw exception;
	}

	return _files[index];
}

#endif

