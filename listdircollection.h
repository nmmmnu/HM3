#ifndef _LIST_DIR_COLLECTION_H
#define _LIST_DIR_COLLECTION_H

#include "ilistcollection.h"
#include "disktable.h"

#include <deque>

class ListDirCollection : public IListCollection{
public:
	ListDirCollection() = default;

	ListDirCollection(const char *path){
		open(path);
	}

	bool open(const char *path);
	void close();

private:
	virtual const ITable & _getAt(uint64_t index) const override;
	virtual uint64_t _getCount() const override;

private:
	std::deque<DiskTable> _files;

};

#endif

