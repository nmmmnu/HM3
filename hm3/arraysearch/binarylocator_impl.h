namespace hm3{
namespace arraysearch{


template<general_size_type MINIMUM_DISTANCE>
template <class ARRAY>
result_type<ARRAY> HybridBinaryLocator<MINIMUM_DISTANCE>::operator()(const ARRAY &list, const StringRef &key,
							typename ARRAY::size_type start,
							typename ARRAY::size_type end) const{
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	int cmp = 0;

	// while (start < end)
	while (start + MINIMUM_DISTANCE < end){
		using size_type = typename ARRAY::size_type;

	//	size_type mid = start + ((end - start) /  2);
		size_type mid = size_type(start + ((end - start) >> 1));

		cmp = list.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			return mid;
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


	return locator_(list, key, start, end);

	// index = start; return cmp;
//	return {false, start};
}


} // namespace arraysearch
} // namespace

#if 0
			if (SCATTER){
				for(;start < start + MINIMUM_DISTANCE; ++start){
					cmp = list.cmpAt(start, key);
					if (cmp == 0){
						// found...
						return start;
					}

					if (cmp > 0){
						// not in the list
						return {false, start};
					}
				}
			}
#endif

