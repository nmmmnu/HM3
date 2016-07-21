namespace hm3{
namespace arraysearch{


template<class LOOKUP>
template <class ARRAY>
void Sampler<LOOKUP>::operator()(const ARRAY &list){
	if (list.getSize() < MIN_SIZE)
		return;

	samples_.reserve(SAMPLES);

	using count_type = typename ARRAY::count_type;

	count_type const step = list.getSize() / SAMPLES;

	// we will skip first one at position 0
	for(count_type i = 1; i < step; ++i){
		count_type const pos = i * step;

		const auto &p = list.getAt(pos);

		samples_.push_back(p.getKey());
	}
}


} // namespace arraysearch
} // namespace

