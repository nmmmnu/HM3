#include "stlvectorlist.h"


namespace hm3{


template <class LOCATOR>
template <class UPAIR>
bool STLVectorList<LOCATOR>::putT_(UPAIR&& newdata){
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
			auto ptr = container_.cbegin() + (difference_type) lr.get();
			container_.insert(ptr, std::forward<UPAIR>(newdata));
		}
	}catch(...){
		return false;
	}

	return true;
}

template <class LOCATOR>
bool STLVectorList<LOCATOR>::remove(const StringRef &key){
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

template class STLVectorList<arraysearch::LinearLocator>;

template bool STLVectorList<arraysearch::LinearLocator>::putT_(Pair &&newdata);
template bool STLVectorList<arraysearch::LinearLocator>::putT_(const Pair &newdata);

template class STLVectorList<arraysearch::BinaryLocator>;

template bool STLVectorList<arraysearch::BinaryLocator>::putT_(Pair &&newdata);
template bool STLVectorList<arraysearch::BinaryLocator>::putT_(const Pair &newdata);

template class STLVectorList<arraysearch::JumpLocator>;

template bool STLVectorList<arraysearch::JumpLocator>::putT_(Pair &&newdata);
template bool STLVectorList<arraysearch::JumpLocator>::putT_(const Pair &newdata);


} // namespace

