#ifndef _IARRAY_H
#define _IARRAY_H

#include "ilist.h"

#include <tuple>

template <class T>
class IArray{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

	// propagating count_type...
	//typedef typename IList<IArray<T> >::count_type count_type;

	typedef IListDefs::count_type count_type;

public:
	Pair get(const StringRef &key) const;

	std::tuple<int, count_type> lookup(const StringRef &key) const;

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
	uint8_t		_lookupMethod = BINARY_SEARCH;

private:
	std::tuple<int, count_type> _lookupBinarySearch(const StringRef &key) const;
	std::tuple<int, count_type> _lookupLinearSearch(const StringRef &key) const;

};

// ==============================

template <class T>
auto IArray<T>::lookup(const StringRef &key) const -> std::tuple<int, count_type>{
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
	return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
}

// ==============================

#include "iarray_implementation.h"

#endif

