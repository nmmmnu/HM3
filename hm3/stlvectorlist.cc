#include "stlvectorlist.h"


namespace hm3{


template <class LOOKUP>
template <class UPAIR>
bool STLVectorList<LOOKUP>::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	const auto &lr = lookup(key);

	if (lr){
		// key exists, overwrite, do not shift

		Pair & olddata = container_[ lr.get() ];

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		dataSize_ = dataSize_
					- olddata.getSize()
					+ newdata.getSize();

		// copy assignment
		olddata = std::forward<UPAIR>(newdata);

		return true;
	}

	// key not exists
	dataSize_ += newdata.getSize();

	try{
		if (lr.get() == container_.size()){
			// push_back micro-optimization :)
			container_.push_back(std::forward<UPAIR>(newdata));
		}else{
			// This is slow, might shiftR
			auto ptr = container_.begin() + (difference_type) lr.get();
			container_.insert(ptr, std::forward<UPAIR>(newdata));
		}
	}catch(...){
		return false;
	}

	return true;
}

template <class LOOKUP>
bool STLVectorList<LOOKUP>::remove(const StringRef &key){
	const auto &lr = lookup(key);

	if (! lr){
		// the key does not exists in the vector.
		return true;
	}

	// Fix size
	Pair & data = container_[ lr.get() ];
	dataSize_ -= data.getSize();

	// This is slow, might shiftL
	auto ptr = container_.begin() + (difference_type) lr.get();
	container_.erase(ptr);

	return true;
}

// ===================================

template class STLVectorList<arraysearch::Linear>;

template bool STLVectorList<arraysearch::Linear>::putT_(Pair &&newdata);
template bool STLVectorList<arraysearch::Linear>::putT_(const Pair &newdata);

template class STLVectorList<arraysearch::Binary>;

template bool STLVectorList<arraysearch::Binary>::putT_(Pair &&newdata);
template bool STLVectorList<arraysearch::Binary>::putT_(const Pair &newdata);


} // namespace

