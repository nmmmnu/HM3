#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <utility>	// std::forward

namespace hm3{

template <class LIST1, class TABLE2>
class DualList : public IMutableList<DualList<LIST1,TABLE2> >{
public:
	using Iterator = MultiTableIterator::Dual<LIST1, TABLE2>;

public:
	DualList(LIST1 &memlist, const TABLE2 &table) :
					memlist_(memlist),
					table_(table){
	}

	DualList(DualList &&other) = default;

public:
	Pair get(const StringRef &key) const{
		const Pair &pair = memlist_.get(key);

		if (pair)
			return pair;

		// lookup into immutable table
		return table_.get(key);
	}

	bool remove(const StringRef &key){
		// remove by inserting tombstone
		return putT_( Pair::tombstone(key) );
	}

	size_t getMemSize() const{
		return memlist_.getMemSize();
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

} // namespace

#endif
