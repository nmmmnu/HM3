template <class CONTAINER>
MultiTableIterator<CONTAINER>::MultiTableIterator(const CONTAINER &container, bool const endIt) :
			_container(container){

	_cur.reserve(_container.size());
	_end.reserve(_container.size());

	for(const DiskTable &dt : _container){
		_cur.push_back(std::move( endIt ? dt.end() : dt.begin()));
		_end.push_back(std::move(dt.end()));
	}
}

template <class CONTAINER>
MultiTableIterator<CONTAINER> &MultiTableIterator<CONTAINER>::operator++(){
	// step 1: find minimal in reverse order to find most recent.

	const Pair &p = operator*();

	if (!p){
		return *this;
	}

	auto size = _container.size();

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

template <class CONTAINER>
Pair MultiTableIterator<CONTAINER>::operator*() const{
	Pair result;

	auto size = _container.size();

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
		if (! result){
			result = pair;
			continue;
		}

		// compare and swap pair if is smaller
		if (pair.cmp(result) < 0){
			result = pair;
		}
	}

	return result;
}

template <class CONTAINER>
bool MultiTableIterator<CONTAINER>::operator==(const MultiTableIterator<CONTAINER> &other) const{
	if (&_container != &other._container)
		return false;

	auto size = _container.size();

	for(size_type i = 0; i < size; ++i)
		if (_cur[i] != other._cur[i])
			return false;

	return true;
}

