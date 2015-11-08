
template <class LIST, size_t CAPACITY>
bool HashList<LIST,CAPACITY>::removeAll(){
	for(LIST &list : _container)
		list.removeAll();

	return true;
}

template <class LIST, size_t CAPACITY>
const Pair &HashList<LIST,CAPACITY>::get(const StringRef &key) const{
	const auto index = _getBucketForKey(key);

	if (index == 0)
		return ListDefs::zero;

	return _container[index - 1].get(key);
}

template <class LIST, size_t CAPACITY>
bool HashList<LIST,CAPACITY>::remove(const StringRef &key){
	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].remove(key);
}

template <class LIST, size_t CAPACITY>
auto HashList<LIST,CAPACITY>::getCount(bool const estimated) const noexcept -> count_type{
	count_type result = 0;

	for(const LIST &list : _container)
		result += list.getCount(estimated);

	return result;
}

template <class LIST, size_t CAPACITY>
size_t HashList<LIST,CAPACITY>::getSize() const noexcept{
	size_t result = 0;

	for(const LIST &list : _container)
		result += list.getSize();

	return result;
}

template <class LIST, size_t CAPACITY>
bool HashList<LIST,CAPACITY>::put(const Pair &pair){
	const StringRef &key = pair.getKey();

	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].put(pair);
}

template <class LIST, size_t CAPACITY>
bool HashList<LIST,CAPACITY>::put(Pair &&pair){
	const StringRef &key = pair.getKey();

	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].put(std::move(pair));
}

// ===================================

// DJB Hash function from CDB
template <class LIST, size_t CAPACITY>
unsigned long HashList<LIST,CAPACITY>::_calcHash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	size_t c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}
