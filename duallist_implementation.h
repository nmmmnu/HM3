
template <class LIST1, class TABLE2>
Pair DualList<LIST1, TABLE2>::get(const StringRef &key) const{
	const Pair &pair = _memlist.get(key);

	if (pair)
		return pair;

	// lookup into immutable table
	return _table.get(key);
}


template <class LIST1, class TABLE2>
template <class UPAIR>
bool DualList<LIST1, TABLE2>::_putT(UPAIR &&data){
	bool const result = _memlist.put( std::forward<UPAIR>(data) );

	if (_memlist.getSize() > _maxSize)
		flush();

	return result;
}


template <class LIST1, class TABLE2>
bool DualList<LIST1, TABLE2>::flush(){
	if (_memlist.isEmpty())
		return false;
			
	// do something here
	printf("Flushing data to disk...\n");
	_memlist.removeAll();
	_memlist.print();

	return true;
}

// ===================================
