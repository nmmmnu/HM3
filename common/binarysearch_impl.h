template <class ARRAY, class T>
bool binarySearch(const ARRAY &list, const T &key,
					typename ARRAY::size_type start,
					typename ARRAY::size_type end,
					typename ARRAY::size_type &result){
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	while (start < end){
		using size_type = typename ARRAY::size_type;

	//	size_type mid = start + ((end - start) /  2);
		size_type mid = size_type(start + ((end - start) >> 1));

		int const cmp = list.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			result = mid;
			return true;
		}

		if (cmp < 0){
			// go right
			start = (size_type) (mid + 1);

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

