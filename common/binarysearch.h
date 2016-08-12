#ifndef _BINARY_SEARCH_H
#define _BINARY_SEARCH_H

//
// ARRAY must have only
// cmpAt(index, key)
//
template <class ARRAY, class T>
bool binarySearch(const ARRAY &list, const T &key,
					typename ARRAY::size_type start,
					typename ARRAY::size_type end,
					typename ARRAY::size_type &result);

template <class ARRAY, class T>
bool binarySearch(const ARRAY &list, const T &key,
					typename ARRAY::size_type size,
					typename ARRAY::size_type &result){
	return binarySearch(list, key, 0, size, result);
}

#include "binarysearch_impl.h"

#endif

