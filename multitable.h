#ifndef _MULTI_TABLE_H
#define _MULTI_TABLE_H

#include "ilist.h"

#include "disktable.h"

template <class CONTAINER>
class MultiTable : public IList<MultiTable<CONTAINER> >{
private:
	typedef typename IList<MultiTable<CONTAINER> >::count_type count_type;

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

private:
	const CONTAINER	&_container;
};

#include "multitable_implementation.h"

#endif

