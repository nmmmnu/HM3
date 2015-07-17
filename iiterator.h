#ifndef _IITERATOR_H
#define _IITERATOR_H

#include <stdint.h>
//#include <stdio.h>

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	inline const Pair first(const char *key = nullptr);
	inline const Pair current();
	inline const Pair next();
	
	inline void invalidate();

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);

protected:
	virtual void _rewind(const char *key = nullptr) = 0;
	virtual const void *_next() = 0;

private:
	Pair _current = nullptr;
	bool _valid = false;
};

// ==============================

inline const Pair IIterator::first(const char *key){
	_rewind(key);
	_valid = true;
	return next();
}

inline const Pair IIterator::current(){
	// _current is "false" no matter of _valid
	return _current;
}

inline const Pair IIterator::next(){
	if (_valid == false){
		// _current is "false" no matter of _valid
		return _current;
	}

	_current = _next();
	//printf(">>> %p\n", _current);

	return _current;
}

inline void IIterator::invalidate(){
	_valid = false;
	_current = nullptr;
}


#endif
