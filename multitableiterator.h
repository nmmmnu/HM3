#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "iiterator.h"

#include <vector>


template <class IT>
class MultiIteratorMatrix{
public:
	MultiIteratorMatrix(IT &&cur, IT &&end) : cur(cur), end(end){}
	
public:
	bool incrementIfSame(const Pair &model);
	const Pair &operator *() const;

public:
	IT cur;
	IT end;
};


// ===================================


template <class TABLE1, class TABLE2>
class DualIterator : public IIterator<DualIterator<TABLE1, TABLE2> >{
private:
	using Iterator1		= typename TABLE1::Iterator;
	using Iterator2		= typename TABLE2::Iterator;

	using IteratorMatrix1	= MultiIteratorMatrix<Iterator1>;
	using IteratorMatrix2	= MultiIteratorMatrix<Iterator2>;

public:
	DualIterator(const TABLE1 table1, const TABLE2 table2, bool endIt = false);

	DualIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const DualIterator &other) const;

private:
	IteratorMatrix1	_it1;
	IteratorMatrix2	_it2;
	
	bool	_internalError = false;
};


// ===================================


template <class CONTAINER_LIST>
class MultiTableIterator : public IIterator<MultiTableIterator<CONTAINER_LIST> >{
private:
	using Iterator		= typename CONTAINER_LIST::value_type::Iterator;
	using IteratorMatrix	= MultiIteratorMatrix<Iterator>;
	using vector_type	= std::vector<IteratorMatrix>;
	
	using size_type		= typename vector_type::size_type;

public:
	MultiTableIterator(const CONTAINER_LIST &list, bool endIt = false);

	MultiTableIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const MultiTableIterator &other) const;

private:
	vector_type	_it;

	bool		_internalError = false;
};


// ===================================


#include "multitableiterator_implementation.h"

#endif

