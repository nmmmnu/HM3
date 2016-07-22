namespace hm3{
namespace arraysearch{



template <class ARRAY>
result_type<ARRAY> LinearLocator::operator()(const ARRAY &self, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type const right) const{
	int cmp = -1;

	for(; left < right; ++left){
		cmp = self.cmpAt(left, key);

		if (cmp == 0){
			// found
			// index = left; return 0;
			return left;
		}

		if (cmp > 0)
			break;
	}

	// index = left; return cmp;
	return {false, left};
}


} // namespace arraysearch
} // namespace
