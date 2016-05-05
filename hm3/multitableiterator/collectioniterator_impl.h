namespace hm3{
namespace multitableiterator{


template <class CONTAINER>
CollectionIterator<CONTAINER>::CollectionIterator(const CONTAINER &list, bool const endIt){
	it_.reserve(list.size());

	// CONTAINER is responsible for ordering the tables,
	// in correct (probably reverse) order.
	for(const auto &table : list)
		it_.push_back({ table, endIt });

	tmp_index.reserve(list.size());
}

template <class CONTAINER>
auto CollectionIterator<CONTAINER>::operator++() -> CollectionIterator &{
	// get cached...
	const Pair &p = tmp_pair ? *tmp_pair : operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	// step 2: increase all duplicates from the index
	for(const auto index : tmp_index)
		++it_[index];

	tmp_pair = nullptr;

	return *this;
}

template <class CONTAINER>
const Pair &CollectionIterator<CONTAINER>::operator*() const{
	if (tmp_pair)
		return *tmp_pair;

	// step 1: find first minimal add other minimals into index
	for(size_type i = 0; i < it_.size(); ++i){
		const Pair &pair = *it_[i];

		// skip if is null
		if ( ! pair )
			continue;

		// initialize
		if (tmp_pair == nullptr){
			_tmp_pairUpdate(i, &pair);

			continue;
		}

		int const cmp = pair.cmp(*tmp_pair);

		// if is smaller, swap
		if (cmp < 0){
			_tmp_pairUpdate(i, &pair);

			continue;
		}

		if (cmp == 0){
			_tmp_pairUpdate(i);

			// if newer, swap
			if ( pair.cmpTime(*tmp_pair) > 0 )
				tmp_pair = &pair;
		}
	}

	return tmp_pair ? *tmp_pair : Pair::zero();
}

template <class CONTAINER>
bool CollectionIterator<CONTAINER>::operator==(const CollectionIterator &other) const{
	if (it_.size() != other.it_.size())
		return false;

	if (_internalError || other._internalError)
		return true;

	for(size_type i = 0; i < it_.size(); ++i){
		if (it_[i] != other.it_[i])
			return false;
	}

	return true;
}

template <class CONTAINER>
void CollectionIterator<CONTAINER>::_tmp_pairUpdate(size_type const index, const Pair *pair) const{
	if (pair){
		tmp_pair = pair;
		tmp_index.clear();
	}

	tmp_index.push_back(index);
}


} // namespace multitableiterator
} // namespace

