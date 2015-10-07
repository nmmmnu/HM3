#ifndef _IARRAY_H
#define _IARRAY_H

#include "itable.h"

class IArray : virtual public ITable{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

	typedef std::pair<int, count_type> locator;

public:
	Pair getAt(uint64_t index) const;
	Pair operator[](uint64_t index) const;
	locator lookup(const StringRef &key) const;

	void setLookupMethod(uint8_t lookupMethod);

private:
	virtual Pair _getAt(uint64_t index) const = 0;

	virtual int _cmpAt(uint64_t index, const StringRef &key) const = 0;

private:
	Pair _get(const StringRef &key) const final;

	std::unique_ptr<IIterator> _getIterator() const final;

private:
	uint8_t		_lookupMethod = BINARY_SEARCH;

private:
	locator _lookupBinarySearch(const StringRef &key) const;
	locator _lookupLinearSearch(const StringRef &key) const;

protected:
	int _cmpAtNaive(uint64_t index, const StringRef &key) const;

};

// ==============================

inline Pair IArray::getAt(uint64_t const index) const{
	// if we check for getCount() here,
	// there will be virtual dispatch.
	// return index < getCount() ? _getAt(index) : nullptr;
	return _getAt(index);
}

inline Pair IArray::operator[](uint64_t const index) const{
	return getAt(index);
}

inline void IArray::setLookupMethod(uint8_t const lookupMethod){
	_lookupMethod = lookupMethod;
}

inline IArray::locator IArray::lookup(const StringRef &key) const{
	// until there are only 2 methods
	if (_lookupMethod == LINEAR_SEARCH)
		return _lookupLinearSearch(key);
	else
		return _lookupBinarySearch(key);
}

// ==============================

inline int IArray::_cmpAtNaive(uint64_t const index, const StringRef &key) const{
	// this is done using virtual dispatch,
	// also will do copy
	return getAt(index).cmp(key);
}

inline Pair IArray::_get(const StringRef &key) const{
	const auto l = lookup(key);
	return l.first ? nullptr : _getAt(l.second);
}

#endif

