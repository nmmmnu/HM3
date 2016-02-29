#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <utility>	// std::forward

template <class LIST1, class TABLE2>
class DualList : public IMutableList<DualList<LIST1,TABLE2> >{
public:
	using Iterator = MultiTableIterator::Dual<LIST1, TABLE2>;

public:
	explicit
	DualList(LIST1 &memlist, const TABLE2 &table, size_t const maxSize = MAX_SIZE) :
					_memlist(memlist),
					_table  (table),
					_maxSize(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	DualList(DualList &&other) = default;

public:
	Pair get(const StringRef &key) const;

	bool remove(const StringRef &key){
		// remove by inserting tombstone
		return _putT( Pair::tombstone(key) );
	}

	size_t getSize() const{
		return _memlist.getSize();
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data){
		return _memlist.put( std::forward<UPAIR>(data) );
	}

public:
	Iterator begin() const{
		return Iterator(_memlist, _table);
	}

	Iterator end() const{
		return Iterator(_memlist, _table, true);
	}

private:
	LIST1		&_memlist;
	const TABLE2	&_table;
};

// ===================================

template <class LIST1, class TABLE2>
Pair DualList<LIST1, TABLE2>::get(const StringRef &key) const{
	const Pair &pair = _memlist.get(key);

	if (pair)
		return pair;

	// lookup into immutable table
	return _table.get(key);
}

#endif
