#include "vectorlist.h"

#include <stdlib.h>	// size_t

//#define xmalloc(s)	malloc(s)
#define xfree(a)	free(a)
#define xrealloc(a, s)	realloc(a, s)


template<typename T>
inline T SGN(const T a){
	if (a == 0)
		return 0;

	return a > 0 ? 1 : -1;
}

VectorList::VectorList(size_t const reallocSize) :
	_reallocSize( reallocSize ? reallocSize : REALLOC_SIZE ) {
	_clear();
}

VectorList::VectorList(VectorList &&other):
		_reallocSize	(other._reallocSize	),
		_buffer		(other._buffer		),
		_bufferReserved	(other._bufferReserved	),
		_dataCount	(other._dataCount	),
		_dataSize	(other._dataSize	){
	other._clear();
}

VectorList::~VectorList(){
	// _buffer may be nullptr, when move constructor is on the way...
	if (_buffer != nullptr)
		removeAll();
}



void VectorList::_removeAll(){
	for(uint64_t i = 0; i < _dataCount; ++i){
		_buffer[i].~Pair();
	}

	_clear(true);
}

bool VectorList::_put(const Pair &newdata){
	auto key = newdata.getKey();

	uint64_t index;
	int cmp = lookup(key, index);

	if (cmp == 0){
		// key exists, overwrite, do not shift

		Pair & olddata = _buffer[index];

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		_dataSize = _dataSize
					- olddata.getSize()
					+ newdata.getSize();

		// copy assignment
		olddata = newdata;

		return true;
	}

	// key not exists, shift, then add
	if ( ! _shiftR(index) ){
		return false;
	}

	_dataSize += newdata.getSize();

	// placement new with copy constructor
	void *placement = & _buffer[index];
	new(placement) Pair(newdata);

	return true;
}

bool VectorList::_remove(const StringRef &key){
	uint64_t index;
	if (lookup(key, index)){
		// the key does not exists in the vector.
		return true;
	}

	// proceed with remove
	Pair & data = _buffer[index];
	_dataSize -= data.getSize();
	data.~Pair();

	_shiftL(index);

	return true;
}

// ===================================

void VectorList::_clear(bool const alsoFree){
	if (alsoFree && _buffer)
		xfree(_buffer);

	_dataCount = 0;
	_dataSize = 0;
	_bufferReserved = 0;
	_buffer = nullptr;
}

bool VectorList::_shiftL(uint64_t const index){
	// this is the most slow operation of them all
	memmove(& _buffer[index], & _buffer[index + 1], (_dataCount - index - 1) * ELEMENT_SIZE);

	_resize(-1);

	return true;
}

bool VectorList::_shiftR(uint64_t const index){
	if (! _resize(1))
		return false;

	// this is the most slow operation of them all
	memmove(& _buffer[index + 1], & _buffer[index], (_dataCount - index - 1) * ELEMENT_SIZE);

	return true;
}

bool VectorList::_resize(int const delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	uint64_t const new_dataCount = _dataCount + SGN(delta);

	if (new_dataCount == 0){
		_clear(true);
		return true;
	}

	size_t const new_bufferReserved = __calcNewSize(new_dataCount, _reallocSize);

	if (_bufferReserved == new_bufferReserved){
		// already resized, done :)
		_dataCount = new_dataCount;

		return true;
	}

	Pair *new_buffer = (Pair *) xrealloc(_buffer, new_bufferReserved * ELEMENT_SIZE);

	if (new_buffer == nullptr)
		return false;

	_dataCount	= new_dataCount;
	_bufferReserved	= new_bufferReserved;
	_buffer		= new_buffer;

	return true;
}

size_t VectorList::__calcNewSize(size_t const size, size_t const reallocSize){
	size_t newsize = size / reallocSize;

	if (size % reallocSize)
		++newsize;

	return newsize * reallocSize;
}

