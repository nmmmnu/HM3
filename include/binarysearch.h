#ifndef BINARY_SEARCH_H_
#define BINARY_SEARCH_H_

struct BinarySearchCompStdandard;	// [] <=>
struct BinarySearchCompList;		// cmpAt(index, key)

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

