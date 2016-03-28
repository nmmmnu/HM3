namespace hm3{

namespace arraysearch{

template <class ARRAY>
CheckedValue<typename ARRAY::count_type, bool, true>	Linear::operator()(const ARRAY &self, const StringRef &key) const{
	auto const count = self.getCount();

	if (count == 0){
		// index = 0; return 1;
		return {false, 0};
	}

	int cmp = -1;

	using count_type = typename ARRAY::count_type;
	count_type i;
	for(i = 0; i < count; ++i){
		cmp = self.cmpAt(i, key);

		if (cmp == 0){
			// found
			// index = i; return 0;
			return i;
		}

		if (cmp > 0)
			break;
	}

	// index = i; return cmp;
	return {false, i};
}

template <class ARRAY>
CheckedValue<typename ARRAY::count_type, bool, true>	Binary::operator()(const ARRAY &self, const StringRef &key) const{
	auto const count = self.getCount();

	if (count == 0){
		// index = 0; return 1;
		return {false, 0};
	}

	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	using count_type = typename ARRAY::count_type;

	count_type start = 0;
	count_type end   = count;
	int cmp = 0;

	while (start < end){
	//	count_type mid = start + ((end - start) /  2);
		count_type mid = (count_type) (start + ((end - start) >> 1));

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

} // namespace IArraySearch

} // namespace

