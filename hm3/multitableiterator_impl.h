namespace hm3{



template<class TABLE>
const Pair &MultiTableIterator::MatrixHelper<TABLE>::operator *() const{
	if (cur == end)
		return Pair::zero();

	return *cur;
}

template<class TABLE>
void MultiTableIterator::MatrixHelper<TABLE>::operator ++(){
	++cur;
}


template<class TABLE>
bool MultiTableIterator::MatrixHelper<TABLE>::operator==(const MatrixHelper &other) const{
	return cur == other.cur && end == other.end;
}

template<class TABLE>
bool MultiTableIterator::MatrixHelper<TABLE>::operator!=(const MatrixHelper &other) const{
	return ! operator==(other);
}


// ===================================


template <class TABLE1, class TABLE2>
MultiTableIterator::Dual<TABLE1, TABLE2>::Dual(const TABLE1 &table1, const TABLE2 &table2, bool const endIt) :
					it1_({ table1, endIt }),
					it2_({ table2, endIt }){
}

#if 0
// straight forward solution 3 comparisons
template <class TABLE1, class TABLE2>
auto MultiTableIterator::Dual<TABLE1, TABLE2>::operator++() -> Dual<TABLE1, TABLE2> &{
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	it1_.incrementIfSame(p);
	it2_.incrementIfSame(p);

	return *this;
}
#endif

// faster solution with 1 comparison.
template <class TABLE1, class TABLE2>
auto MultiTableIterator::Dual<TABLE1, TABLE2>::operator++() -> Dual<TABLE1, TABLE2> &{
	const Pair &pair1 = *it1_;
	const Pair &pair2 = *it2_;

	const auto cmp = pair1.cmp(pair2);

	if (cmp <= 0)
		++it1_;

	if (cmp >= 0)
		++it2_;

	return *this;
}

template <class TABLE1, class TABLE2>
const Pair &MultiTableIterator::Dual<TABLE1, TABLE2>::operator*() const{
	const Pair &pair1 = *it1_;
	const Pair &pair2 = *it2_;

	int const cmp = pair1.cmp(pair2);

	if (cmp == 0){
		// return newer
		return pair1.cmpTime(pair2) > 0 ? pair1 : pair2;
	}

	// return smaller
	return  cmp < 0 ? pair1 : pair2;
}

template <class TABLE1, class TABLE2>
bool MultiTableIterator::Dual<TABLE1, TABLE2>::operator==(const Dual<TABLE1, TABLE2> &other) const{
	if (_internalError)
		return true;

	return it1_ == other.it1_ && it2_ == other.it2_;
}


// ===================================


template <class CONTAINER>
MultiTableIterator::Collection<CONTAINER>::Collection(const CONTAINER &list, bool const endIt){
	it_.reserve(list.size());

	// CONTAINER is responsible for ordering the tables,
	// in correct (probably reverse) order.
	for(const auto &table : list)
		it_.push_back({ table, endIt });

	tmp_index.reserve(list.size());
}

template <class CONTAINER>
auto MultiTableIterator::Collection<CONTAINER>::operator++() -> Collection<CONTAINER> &{
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
const Pair &MultiTableIterator::Collection<CONTAINER>::operator*() const{
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
bool MultiTableIterator::Collection<CONTAINER>::operator==(const Collection<CONTAINER> &other) const{
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
void MultiTableIterator::Collection<CONTAINER>::_tmp_pairUpdate(size_type const index, const Pair *pair) const{
	if (pair){
		tmp_pair = pair;
		tmp_index.clear();
	}

	tmp_index.push_back(index);
}


} // namespace

