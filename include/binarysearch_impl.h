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

		int const cmp = list.cmpAt(mid, key);

		// not optimal way, but more clear
		if (cmp == 0){
			// found
			// index = mid; return 0;
			result = mid;
			return true;
		}

		if (cmp < 0){
			// go right
			start = mid + 1;

			// scatter code would go here.
			// but it have negative effect.
		}else{
			// go left
			end = mid;
		}
	}

	// fallback to linear search...
	for(; start < end; ++start){
		int const cmp = list.cmpAt(start, key);

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

