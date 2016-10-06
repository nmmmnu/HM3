#ifndef _DUAL_MULTI_TABLE_ITERATOR_H
#define _DUAL_MULTI_TABLE_ITERATOR_H

#include "basemultitableiterator.h"

namespace hm3{
namespace multitableiterator{


template <class TABLE1, class TABLE2>
class DualIterator : public IIterator<DualIterator<TABLE1, TABLE2> >{
private:
	using IteratorPair1	= multitableiterator_impl::IteratorPair_<TABLE1>;
	using IteratorPair2	= multitableiterator_impl::IteratorPair_<TABLE2>;

public:
	DualIterator(const TABLE1 &table1, const TABLE2 &table2, bool endIt = false);
	DualIterator(const TABLE1 &table1, const TABLE2 &table2, const StringRef &key);

	DualIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const DualIterator &other) const;

private:
	IteratorPair1	it1_;
	IteratorPair2	it2_;
};


} // namespace multitableiterator
} // namespace


#include "dualiterator_impl.h"


#endif

