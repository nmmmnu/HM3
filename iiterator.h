#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>

#include "pair.h"

class IIterator{
public:
	virtual ~IIterator(){};

	virtual const Pair *first(const char *key = NULL) = 0;
	virtual const Pair *next() = 0;

	void print(uint64_t limit = 100);
};

#endif
