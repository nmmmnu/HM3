#include "iarray.h"

template <class ARRAY>
auto IArraySearch::Linear::processing(const ARRAY &self, const StringRef &key) -> std::tuple<int, count_type>{
	auto const count = self.getCount();

	if (count == 0){
		// index = 0; return 1;
		return std::make_pair(1, 0);
	}

	int cmp = -1;

	count_type i;
	for(i = 0; i < count; ++i){
		cmp = self.cmpAt(i, key);

		if (cmp == 0){
			// found
			// index = i; return 0;
			return std::make_pair(0, i);
		}

		if (cmp > 0)
			break;
	}

	// index = i; return cmp;
	return std::make_tuple(cmp, i);
}

template <class ARRAY>
auto IArraySearch::Binary::processing(const ARRAY &self, const StringRef &key) -> std::tuple<int, count_type>{
	auto const count = self.getCount();

	if (count == 0){
		// index = 0; return 1;
		return std::make_pair(1, 0);
	}

	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	count_type start = 0;
	count_type end   = count;
	int cmp = 0;

	while (start < end){
	//	count_type mid = start + ((end - start) /  2);
		count_type mid = start + ((end - start) >> 1);

		cmp = self.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			return std::make_pair(0, mid);
		}

		if (cmp < 0){
			// go right
			start = mid + 1;
		}else{
			// go left
			end = mid;
		}
	}

	// index = start; return cmp;
	return std::make_tuple(cmp, start);
}

