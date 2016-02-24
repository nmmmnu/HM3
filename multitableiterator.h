#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "iiterator.h"

#include <vector>


namespace MultiIteratorTraits{
	template <class IT>
	class IteratorGroup{
	public:
		IteratorGroup(IT cur, IT end) : cur(cur), end(end){}
		
	public:
		IT cur, end;
	};

};


// ===================================


template <class TABLE1, class TABLE2>
class DualIterator : public IIterator<DualIterator<TABLE1, TABLE2> >{
private:
	using Iterator1		= typename TABLE1::Iterator;
	using Iterator2		= typename TABLE2::Iterator;

	using IteratorGroup1	= typename MultiIteratorTraits::IteratorGroup<Iterator1>;
	using IteratorGroup2	= typename MultiIteratorTraits::IteratorGroup<Iterator2>;

public:
	DualIterator(const TABLE1 table1, const TABLE2 table2, bool endIt = false);

	DualIterator &operator++();

	const Pair &operator*() const;

	bool operator==(const DualIterator &other) const;

private:
	IteratorGroup1	_it1;
	IteratorGroup2	_it2;
	
	bool	_internalError = false;
};


// ===================================


template <class CONTAINER_LIST>
class MultiTableIterator : public IIterator<MultiTableIterator<CONTAINER_LIST> >{
private:
	using Iterator		= typename CONTAINER_LIST::value_type::Iterator;
	using IteratorGroup	= MultiIteratorTraits::IteratorGroup<Iterator>;
	using vector_type	= std::vector<IteratorGroup>;
	
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

#include "multitableiterator_implementation.h"

#endif

