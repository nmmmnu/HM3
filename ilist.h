#ifndef _ILIST_H
#define _ILIST_H

#include "irolist.h"
#include "std/std_container.h"

class IList : virtual public IROList{
public:
	virtual void removeAll() = 0;

	virtual bool put(const Pair *pair) = 0;
	virtual bool remove(const char *key) = 0;
};

#endif

