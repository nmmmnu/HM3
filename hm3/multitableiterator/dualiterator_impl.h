namespace hm3{
namespace multitableiterator{


template <class TABLE1, class TABLE2>
DualIterator<TABLE1, TABLE2>::DualIterator(const TABLE1 &table1, const TABLE2 &table2, bool const endIt) :
					it1_(table1, endIt),
					it2_(table2, endIt){
}

template <class TABLE1, class TABLE2>
DualIterator<TABLE1, TABLE2>::DualIterator(const TABLE1 &table1, const TABLE2 &table2, const StringRef &key) :
					it1_(table1, key),
					it2_(table2, key){
}

#if 0
// straight forward solution 3 comparisons
template <class TABLE1, class TABLE2>
auto DualIterator<TABLE1, TABLE2>::operator++() -> DualIterator &{
	const Pair &p = operator*();

	if ( ! p ){
		// notice, there is no increment here !!!
		_internalError = true;
		return *this;
	}

	it1_.incrementIfSame(p);
	it2_.incrementIfSame(p);

	return *this;
}
#endif

// faster solution with 1 comparison.
template <class TABLE1, class TABLE2>
auto DualIterator<TABLE1, TABLE2>::operator++() -> DualIterator &{
	const Pair &pair1 = *it1_;
	const Pair &pair2 = *it2_;

	const auto cmp = pair1.cmp(pair2);

	if (cmp <= 0)
		++it1_;

	if (cmp >= 0)
		++it2_;

	return *this;
}

template <class TABLE1, class TABLE2>
const Pair &DualIterator<TABLE1, TABLE2>::operator*() const{
	const Pair &pair1 = *it1_;
	const Pair &pair2 = *it2_;

	int const cmp = pair1.cmp(pair2);

	if (cmp == 0){
		// return newer
		return pair1.cmpTime(pair2) > 0 ? pair1 : pair2;
	}

	// return smaller
	return  cmp < 0 ? pair1 : pair2;
}

template <class TABLE1, class TABLE2>
bool DualIterator<TABLE1, TABLE2>::operator==(const DualIterator &other) const{
	if (_internalError)
		return true;

	return it1_ == other.it1_ && it2_ == other.it2_;
}


} // namespace multitableiterator
} // namespace

