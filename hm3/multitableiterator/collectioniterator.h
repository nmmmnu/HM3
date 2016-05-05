#ifndef _COLLECTION_MULTI_TABLE_ITERATOR_H
#define _COLLECTION_MULTI_TABLE_ITERATOR_H

#include "basemultitableiterator.h"

#include <vector>

namespace hm3{
namespace multitableiterator{


template <class CONTAINER>
class CollectionIterator :
			private BaseMultiTableIterator,
			public IIterator<CollectionIterator<CONTAINER> >{
private:
	using Table		= typename CONTAINER::value_type;
	using Matrix		= MatrixHelper_<Table>;
	using vector_type	= std::vector<Matrix>;

	using size_type		= typename vector_type::size_type;

public:
	CollectionIterator(const CONTAINER &list, bool endIt = false);

	CollectionIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const CollectionIterator &other) const;

private:
	void _tmp_pairUpdate(size_type index, const Pair *pair = nullptr) const;

private:
	vector_type		it_;

	bool			_internalError	= false;

	/* !!! */ mutable
	const Pair		*tmp_pair	= nullptr;

	/* !!! */ mutable
	std::vector<size_type>	tmp_index;
};


} // namespace multitableiterator
} // namespace


#include "collectioniterator_impl.h"


#endif

