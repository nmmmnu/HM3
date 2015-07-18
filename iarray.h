#ifndef _IARRAY_H
#define _IARRAY_H

#include "irolist.h"

class IArray : virtual public IROList{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

public:
	inline Pair getAt(uint64_t index) const;

	virtual Pair get(const char *key) const override;

public:
	inline Pair operator[](uint64_t index) const;

	inline int lookup(const char *key, uint64_t &index) const;

	inline void setLookupMethod(uint8_t lookupMethod);

protected:
	virtual void _rewind(const char *key = nullptr) override;
	virtual const void *_next() override;
	virtual const void *_getAt(uint64_t index) const = 0;

private:
	uint8_t		_lookupMethod = BINARY_SEARCH;

	uint64_t	_itPos = 0;

protected:
	int _lookupBinSearch(   const char *key, uint64_t &index) const;
	int _lookupLinearSearch(const char *key, uint64_t &index) const;

};

// ==============================


inline Pair IArray::getAt(uint64_t index) const{
	return _getAt(index);
}

inline Pair IArray::operator[](uint64_t index) const{
	return _getAt(index);
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

