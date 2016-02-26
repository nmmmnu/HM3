
template <class LIST, class FLUSH>
template <class UPAIR>
bool FlushList<LIST, FLUSH>::_putT(UPAIR &&data){
	bool const result = _list.put( std::forward<UPAIR>(data) );

	if (_list.getSize() > _maxSize)
		flush();

	return result;
}

