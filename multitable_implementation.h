
template <class CONTAINER>
Pair MultiTable<CONTAINER>::get(const StringRef &key) const{
	for(const DiskTable &dt : _container){
		if (Pair pair = dt.get(key))
			return pair;
	}

	return nullptr;
}

// ===================================

template <class CONTAINER>
MultiTable<CONTAINER>::Iterator::Iterator(const CONTAINER &container, bool const endIt) :
			_container(container){

	_cur.reserve(_container.size());
	_end.reserve(_container.size());

	for(const DiskTable &dt : _container){
		_cur.push_back(std::move( endIt ? dt.end() : dt.begin()));
		_end.push_back(std::move(dt.end()));
	}

	if (!endIt){
		// rewind iterator...
		operator++();
	}
}

template <class CONTAINER>
auto MultiTable<CONTAINER>::Iterator::operator++() -> Iterator &{
	auto size = _container.size();

	_pair = nullptr;

	// step 1: find minimal in reverse order to find most recent.

	for(size_type i = 0; i < size; ++i ){
		const Pair &pair = *(_cur[i]);

		// skip if is null
		if (! pair)
			continue;

		// initialize
		if (! _pair){
			_pair = std::move(pair);
			continue;
		}

		// compare and swap pair
		if (pair.cmp(_pair) < 0){
			_pair = std::move(pair);
		}
	}

	if (!_pair){
		return *this;
	}

	// step 2: increase all duplicates
	for(size_type i = 0; i < size; ++i )
		if (_cur[i] != _end[i]){
			const Pair &pair = *(_cur[i]);

			if (pair.cmp(_pair) == 0)
				++(_cur[i]);
		}

	return *this;
}

template <class CONTAINER>
const Pair &MultiTable<CONTAINER>::Iterator::operator*() const{
	return _pair;
}

template <class CONTAINER>
bool MultiTable<CONTAINER>::Iterator::operator==(const Iterator &other) const{
	if (&_container == &other._container)
		return false;

	// you can simply check if all iterators == end(),
	// but this is the way it should be done for correctness.

	auto size = _container.size();

	for(size_type i = 0; i < size; ++i)
		if (_cur[i] != other._cur[i])
			return false;

	return true;
}

template <class CONTAINER>
bool MultiTable<CONTAINER>::Iterator::operator!=(const Iterator &other) const{
	return ! ( *this == other );
}

// ===================================

template <class CONTAINER>
auto MultiTable<CONTAINER>::begin() const -> Iterator{
	return Iterator(_container);
}

template <class CONTAINER>
auto MultiTable<CONTAINER>::end() const -> Iterator{
	return Iterator(_container, true);
}

