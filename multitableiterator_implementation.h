
template<class IT>
bool MultiIteratorMatrix<IT>::incrementIfSame(const Pair &model){
	if (cur == end)
		return false;
	
	const Pair &pair = *cur;

	if (pair.cmp(model) != 0)
		return false;
	
	// increase if is same

	++cur;
	
	return true;
}


template<class IT>
const Pair &MultiIteratorMatrix<IT>::operator *() const{
	if (cur == end)
		return Pair::zero();
 
	return *cur;
}


// ===================================


template <class TABLE1, class TABLE2>
DualIterator<TABLE1, TABLE2>::DualIterator(const TABLE1 table1, const TABLE2 table2, bool const endIt) :
					_it1(IteratorMatrix1{
						endIt ? table1.end() : table1.begin(),
						table1.end()
					}),
					_it2(IteratorMatrix2{
						endIt ? table2.end() : table2.begin(),
						table2.end()
					}){
}

template <class TABLE1, class TABLE2>
DualIterator<TABLE1, TABLE2> &DualIterator<TABLE1, TABLE2>::operator++(){
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	_it1.incrementIfSame(p);
	_it2.incrementIfSame(p);

	return *this;
}

template <class TABLE1, class TABLE2>
const Pair &DualIterator<TABLE1, TABLE2>::operator*() const{
	const Pair &pair1 = *_it1;
	const Pair &pair2 = *_it2;

	return pair1.cmp(pair2) < 0 ? pair1 : pair2; 
}

template <class TABLE1, class TABLE2>
bool DualIterator<TABLE1, TABLE2>::operator==(const DualIterator<TABLE1, TABLE2> &other) const{
	if (_internalError)
		return true;
		
	return _it1.cur == other._it1.cur;
}


// ===================================



template <class CONTAINER_LIST>
MultiTableIterator<CONTAINER_LIST>::MultiTableIterator(const CONTAINER_LIST &list, bool const endIt){
	_it.reserve(list.size());

	for(const auto &table : list)
		_it.push_back(IteratorMatrix{
			endIt ? table.end() : table.begin(),
			table.end()
		});
}

template <class CONTAINER_LIST>
MultiTableIterator<CONTAINER_LIST> &MultiTableIterator<CONTAINER_LIST>::operator++(){
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	// step 2: increase all duplicates
	for(auto &iter : _it)
		iter.incrementIfSame(p);

	return *this;
}

template <class CONTAINER_LIST>
const Pair &MultiTableIterator<CONTAINER_LIST>::operator*() const{
	// const refference that can be binded several times :)
	const Pair *resultRef = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(auto &iter : _it){
		const Pair &pair = *iter;

		// skip if is null
		if ( ! pair )
			continue;

		// initialize
		if (resultRef == nullptr){
			resultRef = &pair;
			continue;
		}

		// compare and swap pair if is smaller
		if (pair.cmp(*resultRef) < 0){
			resultRef = &pair;
		}
	}

	return resultRef ? *resultRef : Pair::zero();
}

template <class CONTAINER_LIST>
bool MultiTableIterator<CONTAINER_LIST>::operator==(const MultiTableIterator<CONTAINER_LIST> &other) const{
	if (_it.size() != other._it.size())
		return false;

	if (_internalError)
		return true;

	for(size_type i = 0; i < _it.size(); ++i){
		if (_it[i].cur != other._it[i].cur)
			return false;
	}

	return true;
}

