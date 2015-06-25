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
	virtual const OPair next() = 0;

	inline const OPair first(const char *key = NULL);

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);
};

inline const OPair IIterator::first(const char *key){
	return rewind(key) ? next() : NULL;
}

#endif
