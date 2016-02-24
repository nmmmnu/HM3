#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "multitableiterator.h"

#include <utility>	// std::forward

template <class LSMC>
class LSMTable : public IList<LSMTable<LSMC> >{
public:
	using container_type	= typename LSMC::container_type;

public:
	using count_type	= typename LSMTable::count_type;

public:
	template<class ULSMC>
	LSMTable(ULSMC &&lsmc) : _lsmc(std::forward<ULSMC>(lsmc)){}

public:
	MultiTableIterator<container_type> begin() const{
		return MultiTableIterator<container_type>(*_lsmc);
	}

	MultiTableIterator<container_type> end() const{
		return MultiTableIterator<container_type>(*_lsmc, true);
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
	LSMC	_lsmc;
};

// ===================================

#include "lsmtable_implementation.h"

#endif

