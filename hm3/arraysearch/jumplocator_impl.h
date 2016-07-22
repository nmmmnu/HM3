namespace hm3{
namespace arraysearch{



template <class ARRAY>
result_type<ARRAY> JumpLocator::operator()(const ARRAY &list, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type const right) const{

	auto mid = left + jump_;

	while(mid < right){
		int cmp = list.cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = left; return 0;
			return mid;
		}

		if (cmp > 0){
			//printf("%12zu to %12zu => Fallback to binary\n", left, right);
			return locator_(list, key, left, right);
		}

		left = mid;

		//printf("%12zu to %12zu => %s\n", left, right, list.getAt(left).getKey().data());

		mid += jump_;
	}

	// index = left; return cmp;
	return {false, left};
}


} // namespace arraysearch
} // namespace

