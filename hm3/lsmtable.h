#ifndef _LSM_TABLE_H
#define _LSM_TABLE_H

#include "ilist.h"

#include "multitableiterator/collectioniterator.h"


namespace hm3{


template <class CONTAINER>
class LSMTable : public IList<LSMTable<CONTAINER> >{
public:
	using count_type	= typename LSMTable::count_type;
	using Iterator		= multitableiterator::CollectionIterator<CONTAINER>;

public:
	LSMTable(const CONTAINER &container) : container_(container){}

public:
	Iterator begin() const{
		return Iterator(container_);
	}

	Iterator end() const{
		return Iterator(container_, true);
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
	const CONTAINER	&container_;
};


} //namespace

// ===================================

#include "lsmtable_impl.h"

#endif

