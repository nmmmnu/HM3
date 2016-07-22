namespace hm3{
namespace arraysearch{



template <class ARRAY>
result_type<ARRAY> BinaryLocator::operator()(const ARRAY &self, const StringRef &key,
							typename ARRAY::size_type start,
							typename ARRAY::size_type end) const{
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	int cmp = 0;

	while (start < end){
		using size_type = typename ARRAY::size_type;

	//	size_type mid = start + ((end - start) /  2);
		size_type mid = size_type(start + ((end - start) >> 1));

		cmp = self.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			return mid;
		}

		if (cmp < 0){
			// go right
			start = (size_type) (mid + 1);
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

