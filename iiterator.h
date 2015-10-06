#ifndef _IITERATOR_H
#define _IITERATOR_H

#include "icountable.h"
#include "pair.h"

class IIterator{
public:
	static const uint64_t DEFAULT_PRINT_LIMIT = 100;

	typedef IVersion::version_type version_type;
public:
	virtual ~IIterator() = default;

public:
	Pair first(const StringRef &key = StringRef() );
	Pair current() const;
	Pair next();

	IVersion::version_type getVersion() const;

	uint64_t iteratorCount();
	void print(uint64_t limit = DEFAULT_PRINT_LIMIT);

private:
	virtual void _rewind(const StringRef &key) = 0;
	virtual Pair _next() = 0;
	virtual uint64_t _getVersion() const = 0;

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

inline Pair IIterator::current() const{
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

inline uint64_t IIterator::getVersion() const{
	return _getVersion();
}

#endif
