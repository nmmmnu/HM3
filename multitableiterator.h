#ifndef _MULTI_TABLE_ITERATOR_H
#define _MULTI_TABLE_ITERATOR_H

#include "iiterator.h"

#include <vector>

class MultiTableIterator{
private:
	MultiTableIterator() = delete;

private:
	template <class TABLE>
	class MatrixHelper{
	private:
		using Iterator		= typename TABLE::Iterator;

	public:
		MatrixHelper(Iterator &&cur, Iterator &&end) :
						cur(std::move(cur)),
						end(std::move(end)){}

		MatrixHelper(const TABLE &table, bool const endIt = false) :
						MatrixHelper(
							endIt ? table.end() : table.begin(),
							table.end()
						){}

	public:
		bool incrementIfSame(const Pair &model);
		const Pair &operator *() const;
		bool operator==(const MatrixHelper &other) const;
		bool operator!=(const MatrixHelper &other) const;

	public:
		Iterator cur;
		Iterator end;
	};

public:
	template <class TABLE1, class TABLE2>
	class Dual : public IIterator<Dual<TABLE1, TABLE2> >{
	private:
		using Matrix1		= MatrixHelper<TABLE1>;
		using Matrix2		= MatrixHelper<TABLE2>;

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
	template <class CONTAINER>
	class Collection : public IIterator<Collection<CONTAINER> >{
	private:
		using Table		= typename CONTAINER::value_type;
		using Matrix		= MatrixHelper<Table>;
		using vector_type	= std::vector<Matrix>;

		using size_type		= typename vector_type::size_type;

	public:
		Collection(const CONTAINER &list, bool endIt = false);

		Collection &operator++();

		const Pair &operator*() const;

		bool operator==(const Collection &other) const;

	private:
		vector_type	_it;

		bool		_internalError	= false;

		/* !!! */ mutable
		const Pair	*tmp_pair	= nullptr;
	};
};


// ===================================


#include "multitableiterator_implementation.h"

#endif

