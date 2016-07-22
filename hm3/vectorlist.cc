#include "vectorlist.h"

#define xfree(p)		free(p)
#define xrealloc(p, size)	realloc(p, (size_t) size)

#define xmemmove(d, s, size)	memmove(d, s, (size_t) size)


namespace {

template<typename T>
inline T SGN(const T &a){
	if (a == 0)
		return 0;

	return a > 0 ? 1 : -1;
}

}


namespace hm3{


template <class LOCATOR>
VectorList<LOCATOR>::VectorList(size_type const reallocCount) :
		reallocCount_( reallocCount ? reallocCount : 1 ) {
	clear_();
}

template <class LOCATOR>
VectorList<LOCATOR>::VectorList(VectorList &&other):
		reallocCount_	(std::move(other.reallocCount_		)),
		buffer_		(std::move(other.buffer_		)),
		reservedCount_	(std::move(other.reservedCount_		)),
		dataCount_	(std::move(other.dataCount_		)),
		dataSize_	(std::move(other.dataSize_		)){
	other.clear_();
}

template <class LOCATOR>
bool VectorList<LOCATOR>::removeAll(){
	for(size_type i = 0; i < dataCount_; ++i)
		buffer_[i].~Pair();

	clear_(true);

	return true;
}

template <class LOCATOR>
template <class UPAIR>
bool VectorList<LOCATOR>::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	const auto &lr = lookup(key);

	if (lr){
		// key exists, overwrite, do not shift

		Pair & olddata = buffer_[ lr.get() ];

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

	// key not exists, shift, then add
	if ( ! shiftR_( lr.get() ) ){
		return false;
	}

	dataSize_ += newdata.getSize();

	// placement new with copy constructor
	void *placement = & buffer_[ lr.get() ];
	new(placement) Pair(std::forward<UPAIR>(newdata));

	return true;
}

template <class LOCATOR>
bool VectorList<LOCATOR>::remove(const StringRef &key){
	const auto &lr = lookup(key);

	if (! lr){
		// the key does not exists in the vector.
		return true;
	}

	// proceed with remove
	Pair & data = buffer_[lr.get()];
	dataSize_ -= data.getSize();
	data.~Pair();

	shiftL_(lr.get());

	return true;
}

// ===================================

template <class LOCATOR>
void VectorList<LOCATOR>::clear_(bool const alsoFree){
	if (alsoFree && buffer_)
		xfree(buffer_);

	dataCount_	= 0;
	dataSize_	= 0;
	reservedCount_	= 0;
	buffer_		= nullptr;
}

template <class LOCATOR>
bool VectorList<LOCATOR>::shiftL_(size_type const index){
	// this is the most slow operation of them all
	xmemmove(
		& buffer_[index],
		& buffer_[index + 1],
		(dataCount_ - index - 1) * ELEMENT_SIZE
	);

	resize_(-1);

	return true;
}

template <class LOCATOR>
bool VectorList<LOCATOR>::shiftR_(size_type const index){
	if (! resize_(1))
		return false;

	// this is the most slow operation of them all
	xmemmove(
		& buffer_[index + 1],
		& buffer_[index],
		(dataCount_ - index - 1) * ELEMENT_SIZE
	);

	return true;
}

template <class LOCATOR>
bool VectorList<LOCATOR>::resize_(int const delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	if (dataCount_ == 0 && delta < 0){
		// must be an error
		return true;
	}

	size_type const new_dataCount = dataCount_ + (size_type) SGN(delta);

	if (new_dataCount == 0){
		clear_(true);
		return true;
	}

	size_type const new_reservedCount = calcNewCount_(new_dataCount);

	if (reservedCount_ == new_reservedCount){
		// already resized, done :)
		dataCount_ = new_dataCount;

		return true;
	}

	Pair *new_buffer = (Pair *) xrealloc(buffer_, new_reservedCount * ELEMENT_SIZE);

	if (new_buffer == nullptr)
		return false;

	dataCount_	= new_dataCount;
	reservedCount_	= new_reservedCount;
	buffer_		= new_buffer;

	return true;
}

template <class LOCATOR>
auto VectorList<LOCATOR>::calcNewCount_(size_type const count) -> size_type{
	size_type newsize = count / reallocCount_;

	if (count % reallocCount_)
		++newsize;

	return newsize * reallocCount_;
}

// ===================================

template class VectorList<arraysearch::LinearLocator>;

template bool VectorList<arraysearch::LinearLocator>::putT_(Pair &&newdata);
template bool VectorList<arraysearch::LinearLocator>::putT_(const Pair &newdata);

template class VectorList<arraysearch::BinaryLocator>;

template bool VectorList<arraysearch::BinaryLocator>::putT_(Pair &&newdata);
template bool VectorList<arraysearch::BinaryLocator>::putT_(const Pair &newdata);


} // namespace

