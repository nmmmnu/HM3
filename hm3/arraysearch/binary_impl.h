namespace hm3{
namespace arraysearch{



template <class ARRAY>
result_type<ARRAY> Binary::operator()(const ARRAY &self, const StringRef &key,
							typename ARRAY::count_type start,
							typename ARRAY::count_type end) const{
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	int cmp = 0;

	while (start < end){
		using count_type = typename ARRAY::count_type;

	//	count_type mid = start + ((end - start) /  2);
		count_type mid = count_type(start + ((end - start) >> 1));

		cmp = self.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			return mid;
		}

		if (cmp < 0){
			// go right
			start = (count_type) (mid + 1);
		}else{
			// go left
			end = mid;
		}
	}

	// index = start; return cmp;
	return {false, start};
}


} // namespace arraysearch
} // namespace

