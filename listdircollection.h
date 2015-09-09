#ifndef _LIST_DIR_COLLECTION_H
#define _LIST_DIR_COLLECTION_H

#include "ilistcollection.h"
#include "disktable.h"

#include <vector>
#include <string>

class ListDirCollection : public IListCollection{
public:
	ListDirCollection() = default;

	ListDirCollection(const std::string &path){
		open(path);
	}

	bool open(const std::string &path);
	void close();

private:
	virtual const ITable & _getAt(uint64_t index) const override;
	virtual uint64_t _getCount() const override;

private:
	std::vector<DiskTable> _files;

};

// ==============================

inline const ITable & ListDirCollection::_getAt(uint64_t const index) const{
	return _files[index];
}

inline uint64_t ListDirCollection::_getCount() const{
	return _files.size();
}

// ==============================


#endif

