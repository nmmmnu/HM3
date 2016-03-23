#include "stlvectorlist.h"


namespace hm3{


template <class LOOKUP>
template <class UPAIR>
bool STLVectorList<LOOKUP>::_putT(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	const auto &l = lookup(key);
	const auto cmp   = std::get<0>(l);
	const auto index = std::get<1>(l);

	if (cmp == 0){
		// key exists, overwrite, do not shift

		Pair & olddata = _container[index];

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		_dataSize = _dataSize
					- olddata.getSize()
					+ newdata.getSize();

		// copy assignment
		olddata = std::forward<UPAIR>(newdata);

		return true;
	}

	// key not exists
	_dataSize += newdata.getSize();

	try{
		if (index == _container.size()){
			// push_back micro-optimization :)
			_container.push_back(std::forward<UPAIR>(newdata));
		}else{
			// This is slow, might shiftR
			auto ptr = _container.begin() + index;
			_container.insert(ptr, std::forward<UPAIR>(newdata));
		}
	}catch(...){
		return false;
	}

	return true;
}

template <class LOOKUP>
bool STLVectorList<LOOKUP>::remove(const StringRef &key){
	const auto &l = lookup(key);
	const auto cmp   = std::get<0>(l);
	const auto index = std::get<1>(l);

	if (cmp){
		// the key does not exists in the vector.
		return true;
	}

	// Fix size
	Pair & data = _container[index];
	_dataSize -= data.getSize();

	// This is slow, might shiftL
	auto ptr = _container.begin() + index;
	_container.erase(ptr);

	return true;
}

// ===================================

template class STLVectorList<arraysearch::Linear>;

template bool STLVectorList<arraysearch::Linear>::_putT(Pair &&newdata);
template bool STLVectorList<arraysearch::Linear>::_putT(const Pair &newdata);

template class STLVectorList<arraysearch::Binary>;

template bool STLVectorList<arraysearch::Binary>::_putT(Pair &&newdata);
template bool STLVectorList<arraysearch::Binary>::_putT(const Pair &newdata);


} // namespace

