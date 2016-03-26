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
					memlist_(memlist),
					table_  (table),
					maxSize_(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	DualList(DualList &&other) = default;

public:
	Pair get(const StringRef &key) const;

	bool remove(const StringRef &key){
		// remove by inserting tombstone
		return putT_( Pair::tombstone(key) );
	}

	size_t getSize() const{
		return memlist_.getSize();
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data){
		return memlist_.put( std::forward<UPAIR>(data) );
	}

public:
	Iterator begin() const{
		return Iterator(memlist_, table_);
	}

	Iterator end() const{
		return Iterator(memlist_, table_, true);
	}

private:
	LIST1		&memlist_;
	const TABLE2	&table_;
};

// ===================================

template <class LIST1, class TABLE2>
Pair DualList<LIST1, TABLE2>::get(const StringRef &key) const{
	const Pair &pair = memlist_.get(key);

	if (pair)
		return pair;

	// lookup into immutable table
	return table_.get(key);
}

#endif
