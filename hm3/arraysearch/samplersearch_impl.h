namespace hm3{
namespace arraysearch{


template<class LOOKUP>
template <class ARRAY>
void SamplerSearch<LOOKUP>::populate_(const ARRAY &list){
	populated_ = true;

	if (list.getSize() < MIN_CHUNK_SIZE * samplesCount_){
		printf("Resampling skipped...\n");
		return;
	}

	samples_.reserve(samplesCount_ - 1);

	using size_type = typename ARRAY::size_type;

	size_type const step = list.getSize() / samplesCount_;

	// we skip first one at position 0
	for(size_type i = 1; i < samplesCount_; ++i){
		size_type const pos = i * step;

		const auto &p = list.getAt(pos);

		samples_.push_back(p.getKey());
	}

	printf("Resampling done...\n");
}


template<class LOOKUP>
template <class ARRAY>
result_type<ARRAY> SamplerSearch<LOOKUP>::operator()(const ARRAY &list, const StringRef &key){
	if (populated_ == false)
		populate_(list);

	if (samples_.empty())
		return lookup_(list, key, 0, list.getCount());

	using size_type = typename ARRAY::size_type;

	size_type const step = list.getSize() / samplesCount_;

	size_type left  = 0;

	for(SamplesContainer::size_type i = 0; i < samples_.size(); ++i){
		int const cmp = key.compare( samples_[i] );

		size_type const pos = ( i + 1 ) * step;

		// Key is found in the lookup table
		if (cmp == 0)
			return { true, pos };

		// Key must be in this range.
		if (cmp < 0){
			printf("%12zu - %12zu = %12zu | mid | %s\n", left, pos, pos - left, key.data());
			return lookup_(list, key, left, pos);
		}

		left = pos + 1;
	}

	printf("%12zu - %12zu = %12zu | end | %s\n", left, list.getCount(), list.getCount() - left, key.data());

	// Key must be in last range.
	return lookup_(list, key, left, list.getCount());
}


} // namespace arraysearch
} // namespace

