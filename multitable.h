#ifndef _DIR_TABLE_H
#define _DIR_TABLE_H

#include "ilist.h"

class MultiTableIterator;

class MultiTable : public IList<MultiTable>{
friend class MultiTableIterator;

public:
	MultiTable(IListCollection & collection) : _collection(collection){};

public:
	virtual Pair _get(const StringRef &key) const;

	count_type getCount() const{
		return 0;
	};

	size_t getSize() const{
		return 0;
	}

private:
	IListCollection & _collection;
};

#endif

