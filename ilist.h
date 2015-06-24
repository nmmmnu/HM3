#ifndef _ILIST_H
#define _ILIST_H

#include "irolist.h"

class IList : virtual public IROList{
public:
	virtual void removeAll() = 0;

	virtual bool put(Pair *pair) = 0;
	virtual bool remove(const char *key) = 0;
};

#endif

