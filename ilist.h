#ifndef _ILIST_H
#define _ILIST_H

#include "pair.h"
#include "iiterator.h"

class IList{
public:
	virtual ~IList(){};

	virtual void removeAll() = 0;

	virtual bool put(Pair *pair) = 0;
	virtual const Pair *get(const char *key) const = 0;
	virtual bool remove(const char *key) = 0;

	virtual uint64_t getCount() = 0;
	virtual uint64_t getSize() = 0;

	virtual IIterator *getIterator() = 0;
};

#endif

