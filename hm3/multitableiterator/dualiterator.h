#ifndef _DUAL_MULTI_TABLE_ITERATOR_H
#define _DUAL_MULTI_TABLE_ITERATOR_H

#include "basemultitableiterator.h"

namespace hm3{
namespace multitableiterator{


template <class TABLE1, class TABLE2>
class DualIterator : public IIterator<DualIterator<TABLE1, TABLE2> >{
private:
	using Matrix1	= helpers::MatrixHelper_<TABLE1>;
	using Matrix2	= helpers::MatrixHelper_<TABLE2>;

public:
	DualIterator(const TABLE1 &table1, const TABLE2 &table2, bool endIt = false);
	DualIterator(const TABLE1 &table1, const TABLE2 &table2, const StringRef &key);

	DualIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const DualIterator &other) const;

private:
	Matrix1	it1_;
	Matrix2	it2_;

	bool	_internalError = false;
};


} // namespace multitableiterator
} // namespace


#include "dualiterator_impl.h"


#endif

