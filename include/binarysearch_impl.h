#include "sgn.h"

template <class ARRAY, class SIZE, class KEY, class COMP>
bool binarySearch(const ARRAY &list,
				SIZE start, SIZE end,
				const KEY &key,
				const COMP &comp,
				SIZE &result,
				SIZE const minimum_distance){
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	while (start + minimum_distance < end){
	//	SIZE mid = start + ((end - start) /  2);
		SIZE mid = SIZE(start + ((end - start) >> 1)); // 4% faster

		int const cmp = comp(list, mid, key); //list.cmpAt(mid, key);

		if (cmp < 0){
			// go right
			start = mid + 1;

			// scatter code would go here.
			// but it have negative effect.
		}else if (cmp > 0){
			// go left
			end = mid;
		}else{
			// found
			// index = mid; return 0;
			result = mid;
			return true;
		}

	}

	// fallback to linear search...
	for(; start < end; ++start){
		int const cmp = comp(list, start, key); //list.cmpAt(start, key);

		if (cmp == 0){
			// found
			// index = left; return 0;
			result = start;
			return true;
		}

		if (cmp > 0)
			break;
	}

	result = start;
	return false;
}

// ===================================

template <class T>
int binarySearchComparator(const T &a, const T &b){
	if (a < b){
		return -1;
	}else if (a > b){
		return +1;
	}

	return 0;
}

inline int binarySearchComparator(char const a, char const b){
	return a - b;
}

inline int binarySearchComparator(int const a, int const b){
	return a - b;
}

inline int binarySearchComparator(long int const a, long int const b){
	return sgn(a - b);
}

