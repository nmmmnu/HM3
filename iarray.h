#ifndef _IARRAY_H
#define _IARRAY_H

#include "list.h"

template <class T>
class IArray : public IList<IArray<T> >{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

	// propagating count_type...
	typedef typename IList<IArray<T> >::count_type count_type;

	typedef std::pair<int, count_type> locator;

	class Iterator;

public:
	Pair get(const StringRef &key) const;

	locator lookup(const StringRef &key) const;

	void setLookupMethod(uint8_t const lookupMethod){
		_lookupMethod = lookupMethod;
	}

public:
	Pair getAt(count_type const index) const{
		return static_cast<const T*>(this)->getAt(index);
	}

	int  cmpAt(count_type const index, const StringRef &key) const{
		return static_cast<const T*>(this)->cmpAt(index, key);
	}

	count_type getCount() const{
		return static_cast<const T*>(this)->getCount();
	}

private:
	const Pair &getAtR(count_type const index) const{
		return static_cast<const T*>(this)->getAtR(index);
	}

public:
	Iterator begin() const;
	Iterator end() const;

private:
	uint8_t		_lookupMethod = BINARY_SEARCH;

private:
	locator _lookupBinarySearch(const StringRef &key) const;
	locator _lookupLinearSearch(const StringRef &key) const;

};

// ==============================

template <class T>
auto IArray<T>::lookup(const StringRef &key) const -> locator{
	// until there are only 2 methods
	if (_lookupMethod == LINEAR_SEARCH)
		return _lookupLinearSearch(key);
	else
		return _lookupBinarySearch(key);
}

// ==============================

template <class T>
Pair IArray<T>::get(const StringRef &key) const{
	const auto l = lookup(key);
	return l.first ? nullptr : getAt(l.second);
}

// ==============================

#include "iarray_implementation.h"

#endif

