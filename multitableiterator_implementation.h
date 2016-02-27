
template<class TABLE>
bool MultiTableIterator::MatrixHelper<TABLE>::incrementIfSame(const Pair &model){
	if (cur == end)
		return false;

	const Pair &pair = *cur;

	if (pair.cmp(model) != 0)
		return false;

	// increase if is same

	++cur;

	return true;
}

template<class TABLE>
const Pair &MultiTableIterator::MatrixHelper<TABLE>::operator *() const{
	if (cur == end)
		return Pair::zero();

	return *cur;
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
					_it1({ table1, endIt }),
					_it2({ table2, endIt }){
}

template <class TABLE1, class TABLE2>
auto MultiTableIterator::Dual<TABLE1, TABLE2>::operator++() -> Dual<TABLE1, TABLE2> &{
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	_it1.incrementIfSame(p);
	_it2.incrementIfSame(p);

	return *this;
}

template <class TABLE1, class TABLE2>
const Pair &MultiTableIterator::Dual<TABLE1, TABLE2>::operator*() const{
	const Pair &pair1 = *_it1;
	const Pair &pair2 = *_it2;

	// return smaller,
	// if equal, first have precedence
	return pair1.cmp(pair2) <= 0 ? pair1 : pair2;
}

template <class TABLE1, class TABLE2>
bool MultiTableIterator::Dual<TABLE1, TABLE2>::operator==(const Dual<TABLE1, TABLE2> &other) const{
	if (_internalError)
		return true;

	return _it1 == other._it1 && _it2 == other._it2;
}


// ===================================


template <class CONTAINER>
MultiTableIterator::Collection<CONTAINER>::Collection(const CONTAINER &list, bool const endIt){
	_it.reserve(list.size());

	for(const auto &table : list)
		_it.push_back({ table, endIt });
}

template <class CONTAINER>
auto MultiTableIterator::Collection<CONTAINER>::operator++() -> Collection<CONTAINER> &{
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	// step 2: increase all duplicates
	for(auto &iter : _it)
		if (iter.incrementIfSame(p))
			tmp_pair = nullptr;

	return *this;
}

template <class CONTAINER>
const Pair &MultiTableIterator::Collection<CONTAINER>::operator*() const{
	if (tmp_pair)
		return *tmp_pair;

	// step 1: find minimal in reverse order to find most recent.
	for(const auto &iter : _it){
		const Pair &pair = *iter;

		// skip if is null
		if ( ! pair )
			continue;

		// initialize
		if (tmp_pair == nullptr){
			tmp_pair = &pair;
			continue;
		}

		// compare and swap pair if is smaller
		// if equal last have precedence
		if (pair.cmp(*tmp_pair) < 0){
			tmp_pair = &pair;
		}
	}

	return tmp_pair ? *tmp_pair : Pair::zero();
}

template <class CONTAINER>
bool MultiTableIterator::Collection<CONTAINER>::operator==(const Collection<CONTAINER> &other) const{
	if (_it.size() != other._it.size())
		return false;

	if (_internalError)
		return true;

	for(size_type i = 0; i < _it.size(); ++i){
		if (_it[i] != other._it[i])
			return false;
	}

	return true;
}

