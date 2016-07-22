#ifndef _LSM_TABLE_H
#define _LSM_TABLE_H

#include "ilist.h"

#include "multitableiterator/collectioniterator.h"

#include <algorithm>    // std::count

namespace hm3{


template <class CONTAINER>
class LSMTable : public List<LSMTable<CONTAINER> >{
public:
	using size_type	= typename LSMTable::size_type;
	using Iterator		= multitableiterator::CollectionIterator<CONTAINER>;

public:
	LSMTable(const CONTAINER &container) : container_(container){}

public:
	Iterator getIterator(const StringRef &key) const{
		return Iterator(container_, key);
	}

	Iterator begin() const{
		return Iterator(container_);
	}

	Iterator end() const{
		return Iterator(container_, true);
	}

public:
	Pair get(const StringRef &key) const;

	size_type getCount(bool const estimated = false) const{
		// TODO: difference_type missing...
		// distance(begin(), end());
		return estimated ? getCountEstimated() : getCountReal();
	}

	size_t getSize() const;

private:
	size_type getCountEstimated() const;
	size_type getCountReal() const{
		return std::distance(begin(), end());
	}

private:
	const CONTAINER	&container_;
};


} //namespace

// ===================================

#include "lsmtable_impl.h"

#endif

