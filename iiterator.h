#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	virtual void rewind(const char *key = nullptr) = 0;

	inline const Pair *first(const char *key = nullptr);
	inline const Pair *current();
	inline const Pair *next();

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);

protected:
	virtual const Pair *_next() = 0;

private:
	const Pair *_currIt = nullptr;
};

// ==============================

inline const Pair *IIterator::first(const char *key){
	rewind(key);
	return next();
}

inline const Pair *IIterator::current(){
	return _currIt;
}

inline const Pair *IIterator::next(){
	_currIt = _next();
	return current();
}

#endif
