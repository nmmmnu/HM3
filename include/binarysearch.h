#ifndef BINARY_SEARCH_H_
#define BINARY_SEARCH_H_

template <class T>
int binarySearchComparator(const T &a, const T &b);

int binarySearchComparator(char a, char b);
int binarySearchComparator(int a, int b);
int binarySearchComparator(long int a, long int b);

struct BinarySearchCompStdandard{
	template <class ARRAY, class SIZE, class KEY>
	int operator()(const ARRAY &list, SIZE const index, const KEY &key) const{
		return binarySearchComparator(list[index], key);
	}
};

struct BinarySearchCompList{
	//
	// ARRAY must have only
	// cmpAt(index, key)
	//
	template <class ARRAY, class SIZE, class KEY>
	int operator()(const ARRAY &list, SIZE const index, const KEY &key) const{
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
int binarySearchComparator(long int a, long int b);

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

