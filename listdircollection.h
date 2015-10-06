#ifndef _LIST_DIR_COLLECTION_H
#define _LIST_DIR_COLLECTION_H

#include "ilistcollection.h"
#include "disktable.h"

#include "stringref.h"

#include <vector>

class ListDirCollection : public IListCollection{
public:
	ListDirCollection() = default;

	explicit ListDirCollection(const StringRef &path){
		open(path);
	}

	bool open(const StringRef &path);
	void close();

private:
	const ITable & _getAt(uint64_t index) const final;
	count_type _getCount() const final{
		return _files.size();
	}

private:
	std::vector<DiskTable> _files;

};

// ==============================

inline const ITable & ListDirCollection::_getAt(uint64_t const index) const{
	uint64_t const count = _getCount();

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

