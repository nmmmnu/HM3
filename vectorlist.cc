#include "vectorlist.h"

#include "defs.h"

VectorList::VectorList(size_t reallocSize) :
	_reallocSize(reallocSize ? reallocSize : REALLOC_SIZE) {
	_clear();
}

VectorList::~VectorList(){
	removeAll();
}

void VectorList::removeAll(){
	rewind();

	for(uint64_t i = 0; i < _dataCount; ++i){
		Pair data = _buffer[i];
		data.destroy();
	}

	_clear(true);
}

bool VectorList::put(IPair &newdata){
	rewind();

	const char *key = newdata.getKey();

	uint64_t index;
	int cmp = lookup(key, index);

	if (cmp == 0){
		// key exists, overwrite, do not shift

		Pair olddata = _buffer[index];

		// check if the data in database is valid
		if (! newdata.valid2(olddata) ){
			// prevent memory leak
			newdata.destroy();

			return false;
		}

		_dataSize = _dataSize
					- olddata.getSize()
					+ newdata.getSize();

		olddata.destroy();

		_buffer[index] = (void *) newdata.releaseBlob();

		return true;
	}

	// key not exists, shift, then add
	if ( ! _shiftR(index) ){
		// prevent memory leak
		newdata.destroy();
		return false;
	}

	_dataSize += newdata.getSize();

	_buffer[index] = (void *) newdata.releaseBlob();

	return true;
}

bool VectorList::remove(const char *key){
	rewind();

	uint64_t index;
	if (lookup(key, index)){
		// the key does not exists in the vector.
		return true;
	}

	// proceed with remove
	Pair data = _buffer[index];
	_dataSize -= data.getSize();

	data.destroy();

	_shiftL(index);

	return true;
}

const void *VectorList::getAt(uint64_t index) const{
	return index < _dataCount ? _buffer[index] : nullptr;
}

uint64_t VectorList::getCount() const{
	return _dataCount;
}

size_t VectorList::getSize() const{
	return _dataSize;
}

// ===================================

void VectorList::_clear(bool alsoFree){
	if (alsoFree && _buffer)
		xfree(_buffer);

	_dataCount = 0;
	_dataSize = 0;
	_bufferSize = 0;
	_buffer = nullptr;

	rewind();
}

bool VectorList::_shiftL(uint64_t index){
	// this is the most slow operation of them all
	memmove(& _buffer[index], & _buffer[index + 1], (_dataCount - index - 1) * sizeof(void *));

	_resize(-1);

	return true;
}

bool VectorList::_shiftR(uint64_t index){
	if (! _resize(1))
		return false;

	// this is the most slow operation of them all
	memmove(& _buffer[index + 1], & _buffer[index], (_dataCount - index - 1) * sizeof(void *));

	return true;
}

bool VectorList::_resize(int delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	delta = SGN(delta);

	uint64_t new_dataCount = _dataCount + delta;

	if (new_dataCount == 0){
		_clear(true);
		return true;
	}

	size_t new_bufferSize = __calcNewSize(new_dataCount * sizeof(void *), _reallocSize);

	if (_bufferSize == new_bufferSize){
		// already resized, done :)
		_dataCount = new_dataCount;

		return true;
	}

	void *new_buffer = xrealloc(_buffer, new_bufferSize);

	if (new_buffer == NULL)
		return false;

	_dataCount		= new_dataCount;
	_bufferSize	= new_bufferSize;
	_buffer		= (void **) new_buffer;

	return true;
}

size_t VectorList::__calcNewSize(size_t size, size_t reallocSize){
	size_t newsize = size / reallocSize;

	if (size % reallocSize)
		newsize++;

	return newsize * reallocSize;
}

