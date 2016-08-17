template <class ARRAY, class SIZE, class KEY, class COMP>
bool binarySearch(const ARRAY &list,
				SIZE start, SIZE end,
				const KEY &key,
				const COMP &comp,
				SIZE &result){
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	while (start < end){
	//	SIZE mid = start + ((end - start) /  2);
		SIZE mid = SIZE(start + ((end - start) >> 1)); // 4% faster

		int const cmp = list.cmpAt(mid, key);

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

	result = start;
	return false;
}

