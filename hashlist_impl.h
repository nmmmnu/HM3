namespace hm3{


template <class CONTAINER>
bool HashList<CONTAINER>::removeAll(){
	for(auto &list : container_)
		list.removeAll();

	return true;
}

template <class CONTAINER>
const Pair &HashList<CONTAINER>::get(const StringRef &key) const{
	if (key.empty())
		return Pair::zero();

	const auto index = getBucketForKey_(key);

	return container_[index].get(key);
}

template <class CONTAINER>
bool HashList<CONTAINER>::remove(const StringRef &key){
	if (key.empty())
		return true;

	const auto index = getBucketForKey_(key);

	return container_[index].remove(key);
}

template <class CONTAINER>
auto HashList<CONTAINER>::getCount(bool const estimated) const noexcept -> count_type{
	count_type result = 0;

	for(const auto &list : container_){
		// a + b is int
		result = (count_type) (result + list.getCount(estimated));
	}

	return result;
}

template <class CONTAINER>
size_t HashList<CONTAINER>::getSize() const noexcept{
	size_t result = 0;

	for(const auto &list : container_)
		result += list.getSize();

	return result;
}

template <class CONTAINER>
template <class UPAIR>
bool HashList<CONTAINER>::putT_(UPAIR &&pair){
	if (pair == false)
		return true;

	const StringRef &key = pair.getKey();

	const auto index = getBucketForKey_(key);

	return container_[index].put(std::forward<UPAIR>(pair));
}

// ===================================

// DJB Hash function from CDB
template <class CONTAINER>
unsigned long HashList<CONTAINER>::calcHash_(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	size_t c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}


} // namespace


