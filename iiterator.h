#ifndef _IITERATOR_H
#define _IITERATOR_H

#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

public:
	virtual ~IIterator(){};

	Pair first(const StringRef &key = StringRef() );
	Pair current();
	Pair next();

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);

private:
	virtual void _rewind(const StringRef &key) = 0;
	virtual Pair _next() = 0;
	virtual uint64_t _getVersion() = 0;

private:
	Pair		_current = nullptr;
	uint64_t	_version = 0;
};

// ==============================

inline Pair IIterator::first(const StringRef &key){
	_rewind(key);
	_version = _getVersion();
	return next();
}

inline Pair IIterator::current(){
	if (_version != _getVersion())
		return nullptr;

	return _current;
}

inline Pair IIterator::next(){
	if (_version != _getVersion())
		return nullptr;

	_current = _next();

	return _current;
}

#endif
