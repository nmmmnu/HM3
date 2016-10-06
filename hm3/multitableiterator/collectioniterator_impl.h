namespace hm3{
namespace multitableiterator{

template <class CONTAINER>
template <typename T>
CollectionIterator<CONTAINER>::CollectionIterator(const CONTAINER &list, const T &param, std::nullptr_t){
	it_.reserve(list.size());

	// CONTAINER is responsible for ordering the tables,
	// in correct (probably reverse) order.
	for(const auto &table : list)
		it_.push_back({ table, param });

	tmp_index.reserve(list.size());
}

template <class CONTAINER>
auto CollectionIterator<CONTAINER>::operator++() -> CollectionIterator &{
	const Pair &pair = operator*();

	if ( ! pair ){
		// notice, there is no increment here !!!
		_ended = true;
		return *this;
	}

	// step 2: increase all duplicates from the index
	for(const auto index : tmp_index)
		++it_[index];

	tmp_pair = nullptr;

	return *this;
}

template <class CONTAINER>
bool CollectionIterator<CONTAINER>::operator==(const CollectionIterator &other) const{
	if (it_.size() != other.it_.size())
		return false;

	// ???
	if (_ended || other._ended)
		return true;

	for(size_type i = 0; i < it_.size(); ++i){
		if (it_[i] != other.it_[i])
			return false;
	}

	return true;
}

template <class CONTAINER>
const Pair &CollectionIterator<CONTAINER>::dereference_() const{
	std::vector<size_type>	indexes;
	const Pair		*cpair = nullptr;

	// step 1: find first minimal add other minimals into index
	for(size_type i = 0; i < it_.size(); ++i){
		const Pair &pair = *it_[i];

		// skip if is null
		if ( ! pair )
			continue;

		// initialize
		if (cpair == nullptr){
			indexes.clear();
			indexes.push_back(i);
			cpair = &pair;

			continue;
		}

		int const cmp = pair.cmp(*cpair);

		// if is smaller, start over
		if (cmp < 0){
			indexes.clear();
			indexes.push_back(i);
			cpair = &pair;

			continue;
		}

		// same, add to list.
		if (cmp == 0){
			indexes.push_back(i);

			// if newer, swap
			if ( pair.cmpTime(*cpair) > 0 )
				cpair = &pair;
		}
	}

	// SETTING MUTABLE VARIABLES
	if (cpair){
		tmp_pair = cpair;
		tmp_index = std::move(indexes);

		return *tmp_pair;
	}else{
		tmp_pair = cpair;

		return Pair::zero();
	}
}

} // namespace multitableiterator
} // namespace

