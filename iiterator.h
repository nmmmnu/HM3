#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	virtual const Pair *first(const char *key = NULL) = 0;
	virtual const Pair *next() = 0;

	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);
};

#endif
