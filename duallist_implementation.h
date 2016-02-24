template <class MEMLIST, class TABLE>
Pair DualList<MEMLIST, TABLE>::get(const StringRef &key) const{
	const Pair &pair = _memlist.get(key);

	if (pair)
		return pair;

	// lookup into immutable table
	return _table.get(key);
}


template <class MEMLIST, class TABLE>
template <class UPAIR>
bool DualList<MEMLIST, TABLE>::_putT(UPAIR &&data){
	bool const result = _memlist.put( std::forward<UPAIR>(data) );

	if (_memlist.getSize() > _maxSize)
		flush();

	return result;
}


template <class MEMLIST, class TABLE>
bool DualList<MEMLIST, TABLE>::flush(){
	// do something here
	printf("Flushing data to disk...\n");
	_memlist.removeAll();
	_memlist.print();

	return true;
}

