template <class CONTAINER_LIST>
MultiTableIterator<CONTAINER_LIST>::MultiTableIterator(const CONTAINER_LIST &lists, bool const endIt) :
			_lists(lists){

	_cur.reserve(_lists.size());
	_end.reserve(_lists.size());

	for(const auto &dt : _lists){
		_cur.push_back(std::move( endIt ? dt.end() : dt.begin()));
		_end.push_back(std::move(dt.end()));
	}
}

template <class CONTAINER_LIST>
MultiTableIterator<CONTAINER_LIST> &MultiTableIterator<CONTAINER_LIST>::operator++(){
	// step 1: find minimal in reverse order to find most recent.

	const Pair &p = operator*();

	if (!p){
		return *this;
	}

	auto size = _lists.size();

	// step 2: increase all duplicates
	for(size_type i = 0; i < size; ++i ){
		// do not dereference if is end
		if (_cur[i] == _end[i])
			continue;

		const Pair &pair = *(_cur[i]);

		// increase if is same
		if (pair.cmp(p) == 0)
			++(_cur[i]);
	}

	return *this;
}

template <class CONTAINER_LIST>
const Pair &MultiTableIterator<CONTAINER_LIST>::operator*() const{
	// const refference that can be binded several times :)
	const Pair *resultRef = nullptr;

	const auto size = _lists.size();

	// step 1: find minimal in reverse order to find most recent.
	for(size_type i = 0; i < size; ++i ){
		// do not dereference if is end
		if (_cur[i] == _end[i])
			continue;

		const Pair &pair = *(_cur[i]);

		// skip if is null
		if (! pair)
			continue;

		// initialize
		if (resultRef == nullptr){
			resultRef = &pair;
			continue;
		}

		// compare and swap pair if is smaller
		if (pair.cmp(*resultRef) < 0){
			resultRef = &pair;
		}
	}

	return resultRef ? *resultRef : ListDefs::zero;
}

template <class CONTAINER_LIST>
bool MultiTableIterator<CONTAINER_LIST>::operator==(const MultiTableIterator<CONTAINER_LIST> &other) const{
	if (&_lists != &other._lists)
		return false;

	auto size = _lists.size();

	for(size_type i = 0; i < size; ++i)
		if (_cur[i] != other._cur[i])
			return false;

	return true;
}

