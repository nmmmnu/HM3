namespace hm3{
namespace arraysearch{


template <class ARRAY>
CheckedValue<typename ARRAY::count_type, bool, true>	Linear::operator()(const ARRAY &self, const StringRef &key) const{
	auto const count = self.getCount();

	if (count == 0){
		// index = 0; return 1;
		return {false, 0u};
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



} // namespace arraysearch
} // namespace

