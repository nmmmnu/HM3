
template <class LIST>
bool HashList<LIST>::removeAll(){
	for(LIST &list : _container)
		list.removeAll();

	return true;
}

template <class LIST>
const Pair &HashList<LIST>::get(const StringRef &key) const{
	if (key.empty())
		return Pair::zero();

	const auto index = _getBucketForKey(key);

	return _container[index].get(key);
}

template <class LIST>
bool HashList<LIST>::remove(const StringRef &key){
	if (key.empty())
		return true;

	const auto index = _getBucketForKey(key);

	return _container[index].remove(key);
}

template <class LIST>
auto HashList<LIST>::getCount(bool const estimated) const noexcept -> count_type{
	count_type result = 0;

	for(const LIST &list : _container){
		// a + b is int
		result = (count_type) (result + list.getCount(estimated));
	}

	return result;
}

template <class LIST>
size_t HashList<LIST>::getSize() const noexcept{
	size_t result = 0;

	for(const LIST &list : _container)
		result += list.getSize();

	return result;
}

template <class LIST>
bool HashList<LIST>::put(const Pair &pair){
	const StringRef &key = pair.getKey();

	if (key.empty())
		return true;

	const auto index = _getBucketForKey(key);

	return _container[index].put(pair);
}

template <class LIST>
bool HashList<LIST>::put(Pair &&pair){
	const StringRef &key = pair.getKey();

	if (key.empty())
		return false;

	const auto index = _getBucketForKey(key);

	return _container[index].put(std::move(pair));
}

// ===================================

// DJB Hash function from CDB
template <class LIST>
unsigned long HashList<LIST>::_calcHash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	size_t c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

