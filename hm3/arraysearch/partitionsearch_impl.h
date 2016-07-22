namespace hm3{
namespace arraysearch{


template<class LOOKUP>
template <class ARRAY>
bool PartitionSearch<LOOKUP>::populate(const ARRAY &list) const{
	//using size_type = typename ARRAY::size_type;

	size_type const partitionsSize = list.getCount() / partitionsCount_;

	if (partitionsSize < MIN_PARTITIONS_SIZE){
	//	printf("Prefetching skipped...\n");
		return false;
	}

	partitions_.clear();
	partitions_.reserve(partitionsCount_ - 1);

	size_type i = 0;
	(void) i; // unused

	// skip first partition
	size_t pos = partitionsSize;
	while(pos < list.getCount() - partitionsSize){
		const auto &p = list.getAt(pos);

		partitions_.push_back(p.getKey());

		//printf("%5zu | %14zu to %14zu | %s\n", i++, pos, list.getCount(), (bool) p ? p.getKey().data() : "---empty---");

		pos += partitionsSize;
	}

	populated_ =  true;

	printf("Prefetching done...\n");

	return true;
}


template<class LOOKUP>
template <class ARRAY>
result_type<ARRAY> PartitionSearch<LOOKUP>::operator()(const ARRAY &list, const StringRef &key) const{
	if (populated_ == false)
		this->populate(list);

	if (partitions_.empty())
		return lookup_(list, key, 0, list.getCount());

	size_type const partitionsSize = list.getCount() / partitionsCount_;

	size_type left  = 0;

	for(Container::size_type i = 0; i < partitions_.size(); ++i){
		int const cmp = key.compare( partitions_[i] );

		size_type const pos = ( i + 1 ) * partitionsSize;

		// Key is found in the lookup table
		if (cmp == 0)
			return { true, pos };

		// Key must be in this range.
		if (cmp < 0){
			//printf("%5zu | %14zu to %14zu\n", i, left, pos);

			return lookup_(list, key, left, pos);
		}

		left = pos;
	}

	//printf("----- | %14zu to %14zu\n", left, list.getCount());

	// Key must be in last range.
	return lookup_(list, key, left, list.getCount());
}


} // namespace arraysearch
} // namespace

