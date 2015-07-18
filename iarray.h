#ifndef _IARRAY_H
#define _IARRAY_H

#include "irolist.h"

class IArray : virtual public IROList{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

public:
	Pair getAt(uint64_t index) const;
	Pair operator[](uint64_t index) const;
	int lookup(const char *key, uint64_t &index) const;

	void setLookupMethod(uint8_t lookupMethod);

private:
	virtual Pair _getAt(uint64_t index) const = 0;

	virtual Pair _get(const char *key) const override;

	virtual void _rewind(const char *key = nullptr) override;
	virtual const void *_next() override;

private:
	uint8_t		_lookupMethod = BINARY_SEARCH;

	uint64_t	_itPos = 0;

private:
	int _lookupBinSearch(   const char *key, uint64_t &index) const;
	int _lookupLinearSearch(const char *key, uint64_t &index) const;

};

// ==============================


inline Pair IArray::getAt(uint64_t index) const{
	return index < getCount() ? _getAt(index) : nullptr;
}

inline Pair IArray::operator[](uint64_t index) const{
	return getAt(index);
}

inline void IArray::setLookupMethod(uint8_t lookupMethod){
	_lookupMethod = lookupMethod;
}

inline int IArray::lookup(const char *key, uint64_t &index) const{
	// until there are only 2 methods
	if (_lookupMethod == LINEAR_SEARCH)
		return _lookupLinearSearch(key, index);
	else
		return _lookupBinSearch(key, index);
}

#endif

