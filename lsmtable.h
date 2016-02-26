#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "multitableiterator.h"

template <class LSMC>
class LSMTable : public IList<LSMTable<LSMC> >{
public:
	using container_type	= typename LSMC::container_type;
	using count_type	= typename LSMTable::count_type;
	using Iterator		= MultiTableIterator::Collection<container_type>;

public:
	LSMTable(LSMC &&lsmc) : _lsmc(std::move(lsmc)){}

public:
	Iterator begin() const{
		return Iterator(*_lsmc);
	}

	Iterator end() const{
		return Iterator(*_lsmc, true);
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
		return _lsmc.reload();
	}

private:
	count_type getCountEstimated() const;
	count_type getCountReal() const;

private:
	LSMC	_lsmc;
};

// ===================================

#include "lsmtable_implementation.h"

#endif

