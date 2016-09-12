#include "vectorlist.h"

#include "binarysearch.h"
#include "arrayref.h"

#define xfree(p)		free(p)
#define xrealloc(p, size)	realloc(p, (size_t) size)

#define xmemmove(d, s, size)	memmove(d, s, (size_t) size)

namespace hm3{


VectorList::VectorList(size_type const reallocCount) :
		reallocCount_( reallocCount ? reallocCount : 1 ) {
	clear_();
}

VectorList::VectorList(VectorList &&other):
		reallocCount_	(std::move(other.reallocCount_		)),
		buffer_		(std::move(other.buffer_		)),
		reservedCount_	(std::move(other.reservedCount_		)),
		dataCount_	(std::move(other.dataCount_		)),
		dataSize_	(std::move(other.dataSize_		)){
	other.clear_();
}

bool VectorList::removeAll(){
	for(auto &p : ArrayRef<Pair>{ buffer_, (size_t) dataCount_ } )
		p.~Pair();

	clear_(true);

	return true;
}

inline bool VectorList::binarySearch_(const StringRef &key, size_type &result) const{
	return binarySearch(*this, getCount(), key, BinarySearchCompList{}, result);
}

const Pair &VectorList::get(const StringRef &key) const{
	size_type result;
	bool const found = binarySearch_(key, result);

	return found ? getAt( result ) : Pair::zero();
}

auto VectorList::lowerBound(const StringRef &key) const noexcept -> Iterator{
	size_type result;
	/* bool const found = */ binarySearch_(key, result);
	return buffer_ + result;
}

template <class UPAIR>
bool VectorList::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	size_type result;
	bool const found = binarySearch_(key, result);

	if (found){
		// key exists, overwrite, do not shift

		Pair & olddata = buffer_[ result ];

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

	// key not exists, shift, then add
	if ( ! shiftR_( result ) ){
		return false;
	}

	dataSize_ += newdata.getBytes();

	// placement new with copy constructor
	void *placement = & buffer_[ result ];
	new(placement) Pair(std::forward<UPAIR>(newdata));

	return true;
}

bool VectorList::remove(const StringRef &key){
	size_type result;
	bool const found = binarySearch_(key, result);

	if (! found){
		// the key does not exists in the vector.
		return true;
	}

	// proceed with remove
	Pair & data = buffer_[result];
	dataSize_ -= data.getBytes();
	data.~Pair();

	shiftL_(result);

	return true;
}

// ===================================

void VectorList::clear_(bool const alsoFree){
	if (alsoFree && buffer_)
		xfree(buffer_);

	dataCount_	= 0;
	dataSize_	= 0;
	reservedCount_	= 0;
	buffer_		= nullptr;
}

bool VectorList::shiftL_(size_type const index){
	// this is the most slow operation of them all
	xmemmove(
		& buffer_[index],
		& buffer_[index + 1],
		(dataCount_ - index - 1) * ELEMENT_SIZE
	);

	resize_(-1);

	return true;
}

bool VectorList::shiftR_(size_type const index){
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

bool VectorList::resize_(int const delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	if (dataCount_ == 0 && delta < 0){
		// must be an error
		return true;
	}

	size_type const new_dataCount = dataCount_ + (size_type) sgn(delta);

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

auto VectorList::calcNewCount_(size_type const count) -> size_type{
	size_type newsize = count / reallocCount_;

	if (count % reallocCount_)
		++newsize;

	return newsize * reallocCount_;
}

// ===================================

template bool VectorList::putT_(Pair &&newdata);
template bool VectorList::putT_(const Pair &newdata);

} // namespace

