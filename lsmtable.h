#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "multitableiterator.h"

#include <iterator>

template <class CONTAINER>
class LSMTable : public IList<LSMTable<CONTAINER> >{
public:
	LSMTable(const CONTAINER &container) : _container(container){}

public:
	MultiTableIterator<CONTAINER> begin() const{
		return MultiTableIterator<CONTAINER>(_container);
	}

	MultiTableIterator<CONTAINER> end() const{
		return MultiTableIterator<CONTAINER>(_container, true);
	}

public:
	Pair get(const StringRef &key) const;

	size_t getCount(bool const estimated = false) const{
		// TODO: difference_type missing...
		// distance(begin(), end());
		return estimated ? getCountEstimated() : getCountReal();
	}

	size_t getSize() const;

private:
	size_t getCountEstimated() const;
	size_t getCountReal() const;

private:
	const CONTAINER	&_container;
};

// ===================================

#include "lsmtable_implementation.h"

#endif

