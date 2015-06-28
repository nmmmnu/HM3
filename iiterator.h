#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	virtual bool rewind() = 0;
	virtual bool rewind(const char *key) = 0;
	virtual const void *next() = 0;

	inline const void *first(const char *key = nullptr);

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);
};

// ==============================

inline const void *IIterator::first(const char *key){
	return rewind(key) ? next() : nullptr;
}

#endif
