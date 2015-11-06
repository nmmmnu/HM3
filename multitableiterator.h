#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "disktable.h"

#include <vector>

template <class CONTAINER>
class MultiTableIterator{
private:
	typedef std::vector<typename CONTAINER::value_type::Iterator> vector_type;
	typedef typename vector_type::size_type size_type;

public:
	MultiTableIterator(const CONTAINER &container, bool endIt = false);

	MultiTableIterator &operator++();

	Pair operator*() const;

	bool operator==(const MultiTableIterator &other) const;
	bool operator!=(const MultiTableIterator &other) const{
		return ! ( *this == other );
	}
private:
	const CONTAINER	&_container;

	vector_type	_cur;
	vector_type	_end;
};

#include "multitableiterator_implementation.h"

#endif

