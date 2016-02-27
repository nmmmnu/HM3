#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "multitableiterator.h"

template <class LSMCONTAINER>
class LSMTable : public IList<LSMTable<LSMCONTAINER> >{
public:
	using container_type	= typename LSMCONTAINER::container_type;
	using count_type	= typename LSMTable::count_type;
	using Iterator		= MultiTableIterator::Collection<container_type>;

public:
	LSMTable(const LSMCONTAINER &container) : _container(container){}

public:
	Iterator begin() const{
		return Iterator(*_container);
	}

	Iterator end() const{
		return Iterator(*_container, true);
	}

public:
	Pair get(const StringRef &key) const;

	count_type getCount(bool const estimated = false) const{
		// TODO: difference_type missing...
		// distance(begin(), end());
		return estimated ? getCountEstimated() : getCountReal();
	}

	size_t getSize() const;

	bool reload(){
		return _container.reload();
	}

private:
	count_type getCountEstimated() const;
	count_type getCountReal() const;

private:
	const LSMCONTAINER	&_container;
};

// ===================================

#include "lsmtable_implementation.h"

#endif

