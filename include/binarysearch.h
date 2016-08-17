#ifndef _BINARY_SEARCH_H
#define _BINARY_SEARCH_H

struct BinarySearchStdandard{
	template <class ARRAY, class SIZE, class KEY>
	int operator()(const ARRAY &list, SIZE index, const KEY &key) const{
		if (list[index] == key)
			return 0;

		return list[index] < key ? -1 : +1;
	}
};

struct BinarySearchCompList{
	//
	// ARRAY must have only
	// cmpAt(index, key)
	//
	template <class ARRAY, class SIZE, class KEY>
	int operator()(const ARRAY &list, SIZE index, const KEY &key) const{
		return list.cmpAt(index, key);
	}
};

template <class ARRAY, class SIZE, class KEY, class COMP>
bool binarySearch(const ARRAY &list,
				SIZE start, SIZE end,
				const KEY &key,
				const COMP &comp,
				SIZE &result,
				SIZE minimum_distance = 5);

template <class ARRAY, class SIZE, class KEY, class COMP>
bool binarySearch(const ARRAY &list,
				SIZE const size,
				const KEY &key,
				const COMP &comp,
				SIZE &result,
				SIZE const minimum_distance = 5){
	return binarySearch(list, (SIZE) 0, size, key, comp, result, minimum_distance);
}

#include "binarysearch_impl.h"

#endif

