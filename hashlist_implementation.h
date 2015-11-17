
template <class LIST>
bool HashList<LIST>::removeAll(){
	for(LIST &list : _container)
		list.removeAll();

	return true;
}

template <class LIST>
const Pair &HashList<LIST>::get(const StringRef &key) const{
	const auto index = _getBucketForKey(key);

	if (index == 0)
		return ListDefs::zero;

	return _container[index - 1].get(key);
}

template <class LIST>
bool HashList<LIST>::remove(const StringRef &key){
	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].remove(key);
}

template <class LIST>
size_t HashList<LIST>::getCount(bool const estimated) const noexcept{
	size_t result = 0;

	for(const LIST &list : _container)
		result += list.getCount(estimated);

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

	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].put(pair);
}

template <class LIST>
bool HashList<LIST>::put(Pair &&pair){
	const StringRef &key = pair.getKey();

	const auto index = _getBucketForKey(key);

	if (index == 0)
		return false;

	return _container[index - 1].put(std::move(pair));
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
