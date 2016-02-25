#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "iiterator.h"

#include <vector>

class MultiTableIterator{
private:
	MultiTableIterator() = delete;

private:
	template <class IT>
	class MatrixHelper{
	public:
		MatrixHelper(IT &&cur, IT &&end) : cur(cur), end(end){}

	public:
		bool incrementIfSame(const Pair &model);
		const Pair &operator *() const;
		bool operator==(const MatrixHelper &other) const;
		bool operator!=(const MatrixHelper &other) const;

	public:
		IT cur;
		IT end;
	};

public:
	template <class TABLE1, class TABLE2>
	class Dual : public IIterator<MultiTableIterator::Dual<TABLE1, TABLE2> >{
	private:
		using Iterator1		= typename TABLE1::Iterator;
		using Iterator2		= typename TABLE2::Iterator;

		using Matrix1		= MatrixHelper<Iterator1>;
		using Matrix2		= MatrixHelper<Iterator2>;

	public:
		Dual(const TABLE1 &table1, const TABLE2 &table2, bool endIt = false);

		Dual &operator++();

		const Pair &operator*() const;

		bool operator==(const Dual &other) const;

	private:
		Matrix1	_it1;
		Matrix2	_it2;

		bool	_internalError = false;
	};

public:
	template <class CONTAINER_LIST>
	class Collection : public IIterator<MultiTableIterator::Collection<CONTAINER_LIST> >{
	private:
		using Iterator		= typename CONTAINER_LIST::value_type::Iterator;
		using Matrix		= MatrixHelper<Iterator>;
		using vector_type	= std::vector<Matrix>;

		using size_type		= typename vector_type::size_type;

	public:
		Collection(const CONTAINER_LIST &list, bool endIt = false);

		Collection &operator++();

		const Pair &operator*() const;

		bool operator==(const Collection &other) const;

	private:
		vector_type	_it;

		bool		_internalError = false;
	};
};


// ===================================


#include "multitableiterator_implementation.h"

#endif

