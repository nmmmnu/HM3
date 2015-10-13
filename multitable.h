#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"
#include "multitablecollection.h"

class MultiTable : public IList<MultiTable>{
public:
	MultiTable(MultiTableCollectionDir &collection) : _collection(collection){};

public:
	virtual Pair _get(const StringRef &key) const;

	count_type getCount() const{
		return 0;
	};

	size_t getSize() const{
		return 0;
	}

private:
	MultiTableCollectionDir	&_collection;
};

#endif

