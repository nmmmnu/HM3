#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "iiterator.h"

#include <vector>

template <class CONTAINER_LIST>
class MultiTableIterator : public IIterator<MultiTableIterator<CONTAINER_LIST> >{
private:
	typedef std::vector<typename CONTAINER_LIST::value_type::Iterator> vector_type;
	typedef typename vector_type::size_type size_type;

public:
	MultiTableIterator(const CONTAINER_LIST &lists, bool endIt = false);

	MultiTableIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const MultiTableIterator &other) const;

private:
	const CONTAINER_LIST	&_lists;

	vector_type	_cur;
	vector_type	_end;
};

#include "multitableiterator_implementation.h"

#endif

