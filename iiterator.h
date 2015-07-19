#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>
#include <stdio.h>

#include "pair.h"

#include "iversion.h"

class IIterator : virtual private IVersion{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	Pair first(const char *key = nullptr);
	Pair current();
	Pair next();
	
	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);

private:
	virtual void _rewind(const char *key = nullptr) = 0;
	virtual const void *_next() = 0;

	Pair     _current = nullptr;
	uint64_t _version = 0;
};

// ==============================

inline Pair IIterator::first(const char *key){
	_rewind(key);
	_version = getVersion();
	return next();
}

inline Pair IIterator::current(){
	if (_version != getVersion())
		return nullptr;
	
	return _current;
}

inline Pair IIterator::next(){
	if (_version != getVersion())
		return nullptr;
		
	_current = _next();

	return _current;
}

#endif
