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
		return 0;
	};

	size_t getSize() const{
		return 0;
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

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;
	bool operator!=(const Iterator &other) const;

private:
	const CONTAINER	&_container;

	vector_type	_cur;
	vector_type	_end;

	Pair		_pair;
};

// ===================================

#include "multitable_implementation.h"

#endif

