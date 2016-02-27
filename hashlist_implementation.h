
template <class CONTAINER>
bool HashList<CONTAINER>::removeAll(){
	for(auto &list : _container)
		list.removeAll();

	return true;
}

template <class CONTAINER>
const Pair &HashList<CONTAINER>::get(const StringRef &key) const{
	if (key.empty())
		return Pair::zero();

	const auto index = _getBucketForKey(key);

	return _container[index].get(key);
}

template <class CONTAINER>
bool HashList<CONTAINER>::remove(const StringRef &key){
	if (key.empty())
		return true;

	const auto index = _getBucketForKey(key);

	return _container[index].remove(key);
}

template <class CONTAINER>
auto HashList<CONTAINER>::getCount(bool const estimated) const noexcept -> count_type{
	count_type result = 0;

	for(const auto &list : _container){
		// a + b is int
		result = (count_type) (result + list.getCount(estimated));
	}

	return result;
}

template <class CONTAINER>
size_t HashList<CONTAINER>::getSize() const noexcept{
	size_t result = 0;

	for(const auto &list : _container)
		result += list.getSize();

	return result;
}

template <class CONTAINER>
template <class UPAIR>
bool HashList<CONTAINER>::_putT(UPAIR &&pair){
	if (pair == false)
		return true;

	const StringRef &key = pair.getKey();

	const auto index = _getBucketForKey(key);

	return _container[index].put(std::forward<UPAIR>(pair));
}

// ===================================

// DJB Hash function from CDB
template <class CONTAINER>
unsigned long HashList<CONTAINER>::_calcHash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	size_t c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

