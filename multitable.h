#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "disktable.h"

#include <vector>

template <class CONTAINER>
class MultiTable : public IList<MultiTable<CONTAINER> >{
private:
	typedef typename IList<MultiTable<CONTAINER> >::count_type count_type;

	class Iterator;

public:
	MultiTable(const CONTAINER &container) : _container(container){};

public:
	Pair get(const StringRef &key) const;

	count_type getCount() const{
		auto end_it = end();

		count_type result = 0;

		for(auto it = begin(); it != end_it; ++it)
			++result;

		return result;
	}

	count_type getCountEstimated() const{
		count_type result = 0;

		for(const auto &table : *this)
			result += table.getCount();

		return result;
	}

	size_t getSize() const{
		size_t result = 0;

		for(const auto &table : *this)
			result += table.getSize();

		return result;
	}

public:
	Iterator begin() const;
	Iterator end() const;

private:
	const CONTAINER	&_container;
};

// ===================================

template <class CONTAINER>
class MultiTable<CONTAINER>::Iterator{
friend class MultiTable<CONTAINER>;

private:
	Iterator(const CONTAINER &container, bool endIt = false);

	typedef std::vector<DiskTable::Iterator> vector_type;
	typedef vector_type::size_type size_type;

public:
	Iterator &operator++();

	Pair operator*() const;

	bool operator==(const Iterator &other) const;
	bool operator!=(const Iterator &other) const;

private:
	const CONTAINER	&_container;

	vector_type	_cur;
	vector_type	_end;
};

// ===================================

#include "multitable_implementation.h"

#endif

