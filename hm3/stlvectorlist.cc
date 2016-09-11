#include "stlvectorlist.h"

#include "binarysearch.h"

namespace hm3{

inline bool STLVectorList::binarySearch_(const StringRef &key, size_type &result) const{
	return binarySearch(*this, getBytes(), key, BinarySearchCompList{}, result);
}

const Pair &STLVectorList::get(const StringRef &key) const{
	size_type result;
	bool const found = binarySearch_(key, result);

	return found ? getAt( result ) : Pair::zero();
}

auto STLVectorList::lowerBound(const StringRef &key) const -> Iterator{
	size_type result;
	/* bool const found = */ binarySearch_(key, result);

	return container_.cbegin() + (difference_type) result;
}

template <class UPAIR>
bool STLVectorList::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	size_type result;
	bool const found = binarySearch_(key, result);

	if (found){
		// key exists, overwrite, do not shift

		Pair & olddata = container_[ result ];

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		dataSize_ = dataSize_
					- olddata.getBytes()
					+ newdata.getBytes();

		// copy assignment
		olddata = std::forward<UPAIR>(newdata);

		return true;
	}

	// key not exists
	dataSize_ += newdata.getBytes();

	try{
		if (result == container_.size()){
			// push_back micro-optimization :)
			container_.push_back(std::forward<UPAIR>(newdata));
		}else{
			// This is slow, might shiftR
			auto ptr = container_.cbegin() + (difference_type) result;
			container_.insert(ptr, std::forward<UPAIR>(newdata));
		}
	}catch(...){
		return false;
	}

	return true;
}

bool STLVectorList::remove(const StringRef &key){
	size_type result;
	bool const found = binarySearch_(key, result);

	if (! found){
		// the key does not exists in the vector.
		return true;
	}

	// Fix size
	Pair & data = container_[ result ];
	dataSize_ -= data.getBytes();

	// This is slow, might shiftL
	auto ptr = container_.begin() + (difference_type) result;
	container_.erase(ptr);

	return true;
}

// ===================================

template bool STLVectorList::putT_(Pair &&newdata);
template bool STLVectorList::putT_(const Pair &newdata);

} // namespace

