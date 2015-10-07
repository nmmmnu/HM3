#ifndef _IITERATOR_H
#define _IITERATOR_H

#include "icountable.h"
#include "iversion.h"
#include "pair.h"

class IIterator{
public:
	static const unsigned DEFAULT_PRINT_LIMIT = 100;

	typedef IVersion::version_type version_type;

public:
	IIterator(const IVersion &versionObj) : _versionObj(versionObj){}

	virtual ~IIterator() = default;

public:
	Pair first(const StringRef &key = StringRef() );
	Pair current() const;
	Pair next();

	uint64_t iteratorCount();
	void print(unsigned limit = DEFAULT_PRINT_LIMIT);

private:
	bool _checkVersion() const;

private:
	virtual void _rewind(const StringRef &key) = 0;
	virtual Pair _next() = 0;

private:
	Pair		_current = nullptr;
	const IVersion	&_versionObj;
	version_type	_version = 0;
};

// ==============================

inline bool IIterator::_checkVersion() const{
	return _version != _versionObj.getVersion();
}

inline Pair IIterator::first(const StringRef &key){
	_rewind(key);
	_version = _versionObj.getVersion();
	return next();
}

inline Pair IIterator::current() const{
	if (_checkVersion() )
		return nullptr;

	return _current;
}

inline Pair IIterator::next(){
	if (_checkVersion() )
		return nullptr;

	_current = _next();

	return _current;
}

#endif
