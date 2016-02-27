#ifndef _LSM_TABLE_H
#define _LSM_TABLE_H

#include "ilist.h"

#include "multitableiterator.h"

template <class CONTAINER>
class LSMTable : public IList<LSMTable<CONTAINER> >{
public:
	using count_type	= typename LSMTable::count_type;
	using Iterator		= MultiTableIterator::Collection<CONTAINER>;

public:
	LSMTable(const CONTAINER &container) : _container(container){}

public:
	Iterator begin() const{
		return Iterator(_container);
	}

	Iterator end() const{
		return Iterator(_container, true);
	}

public:
	Pair get(const StringRef &key) const;

	count_type getCount(bool const estimated = false) const{
		// TODO: difference_type missing...
		// distance(begin(), end());
		return estimated ? getCountEstimated() : getCountReal();
	}

	size_t getSize() const;

private:
	count_type getCountEstimated() const;
	count_type getCountReal() const;

private:
	const CONTAINER	&_container;
};

// ===================================

#include "lsmtable_implementation.h"

#endif

