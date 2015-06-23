#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	virtual bool rewind(const char *key = NULL) = 0;
	virtual const Pair *next() = 0;

	inline const Pair *first(const char *key = NULL);

	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);
};

inline const Pair *IIterator::first(const char *key){
	return rewind(key) ? next() : NULL;
}

#endif
