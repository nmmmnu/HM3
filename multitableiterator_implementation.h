
namespace MultiIteratorTraits{
	template<class IT>
	static bool incrementIfSame(IT &it, const IT &it_end, const Pair &model);

	template<class IT>
	static const Pair &dereference(const IT &it, const IT &it_end);
};


// ===================================


template<class IT>
bool MultiIteratorTraits::incrementIfSame(IT &it, const IT &it_end, const Pair &model){
	if (it == it_end)
		return false;
	
	const Pair &pair = *it;

	// increase if is same
	if (pair.cmp(model) != 0)
		return false;
	
	++it;
	return true;
}


template<class IT>
const Pair &MultiIteratorTraits::dereference(const IT &it, const IT &it_end){
	if (it == it_end)
		return Pair::zero();
 
	return *it;
}


// ===================================


template <class TABLE1, class TABLE2>
DualIterator<TABLE1, TABLE2>::DualIterator(const TABLE1 table1, const TABLE2 table2, bool const endIt) :
					_it1(IteratorGroup1{
						std::move(endIt ? table1.end() : table1.begin()),
						std::move(table1.end())
					}),
					_it2(IteratorGroup1{
						std::move(endIt ? table2.end() : table2.begin()),
						std::move(table2.end())
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

	MultiIteratorTraits::incrementIfSame(_it1.cur, _it1.end, p);
	MultiIteratorTraits::incrementIfSame(_it2.cur, _it2.end, p);

	return *this;
}

template <class TABLE1, class TABLE2>
const Pair &DualIterator<TABLE1, TABLE2>::operator*() const{
	const Pair &pair1 = MultiIteratorTraits::dereference(_it1.cur, _it1.end);
	const Pair &pair2 = MultiIteratorTraits::dereference(_it2.cur, _it2.end);

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
		_it.push_back(IteratorGroup{
			std::move(endIt ? table.end() : table.begin()),
			std::move(table.end())
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
		MultiIteratorTraits::incrementIfSame(iter.cur, iter.end, p);

	return *this;
}

template <class CONTAINER_LIST>
const Pair &MultiTableIterator<CONTAINER_LIST>::operator*() const{
	// const refference that can be binded several times :)
	const Pair *resultRef = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(auto &iter : _it){
		const Pair &pair = MultiIteratorTraits::dereference(iter.cur, iter.end);

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

