#include "vectorlist.h"

#define xfree(a)		free(a)
#define xrealloc(a, size)	realloc(a, size)

inline int SGN(int a);

VectorList::VectorList(size_t reallocSize){
	_reallocSize = reallocSize ? reallocSize : REALLOC_SIZE;

	_clear();
}

VectorList::~VectorList(){
	removeAll();
}


void VectorList::removeAll(){
	uint64_t i;
	for(i = 0; i < _count; ++i){
		Pair *data = _buffer[i];

		if (data)
			xfree(data);
	}

	_clear(true);
}

bool VectorList::put(Pair *newdata){
	if (newdata == NULL)
		return false;

	const char *key = newdata->getKey();

	uint64_t index;
	int cmp = _locatePosition(key, & index);

	if (cmp == 0){
		// key exists, overwrite, do not shift

		Pair *olddata = _buffer[index];

		// check if the data in database is valid
		if (! newdata->valid(olddata) ){
			// prevent memory leak
			xfree(newdata);

			return 0;
		}

		_datasize = _datasize
					- olddata->getSize()
					+ newdata->getSize();

		xfree(olddata);

		_buffer[index] = newdata;

		return true;
	}

	if ( ! _shiftR(index) ){
		// prevent memory leak
		xfree(newdata);
		return false;
	}

	_datasize += newdata->getSize();

	_buffer[index] = newdata;

	return true;
}

const Pair *VectorList::get(const char *key){
	if (key == NULL)
		return NULL;

	uint64_t index;
	if (_locatePosition(key, & index)){
		// the key does not exists in the vector.
		return NULL;
	}

	return _buffer[index];
}

bool VectorList::remove(const char *key){
	uint64_t index;
	int cmp = _locatePosition(key, & index);

	if (cmp){
		// the key does not exists in the vector.
		return true;
	}

	// proceed with remove
	Pair *data = _buffer[index];
	_datasize -= data->getSize();

	xfree(data);

	_shiftL(index);

	return true;
}

uint64_t VectorList::getCount(){
	return _count;
}

size_t VectorList::getSize(){
	return _datasize;
}

// ===================================

const Pair *VectorList::first(const char *key){
	if (key){
		uint64_t index;
		if (_locatePosition(key, & index)){
			// the key does not exists in the vector.
			return NULL;
		}

		_itPos = index;
	}else
		_itPos = 0;

	return next();
}

const Pair *VectorList::next(){
	if (_itPos >= _count)
		return NULL;

	return _buffer[_itPos++];
}

// ===================================

void VectorList::_clear(bool alsoFree){
	if (alsoFree && _buffer)
		xfree(_buffer);

	_count = 0;
	_datasize = 0;
	_bufferSize = 0;
	_buffer = NULL;
}

int VectorList::_locatePosition(const char *key, uint64_t *index){
	if (_count == 0){
		*index = 0;
		return 1;
	}

	return _locatePositionBSearch(key, index);
}

int VectorList::_locatePositionBSearch(const char *key, uint64_t *index){
	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	uint64_t start = 0;
	uint64_t end   = _count;
	int cmp = 0;

	while (start < end){
	//	uint64_t mid = start + ((end - start) /  2);
		uint64_t mid = start + ((end - start) >> 1);

		const Pair *data = _buffer[mid];

		cmp = data->cmp(key);

		if (cmp == 0){
			*index = mid;
			return 0;
		}

		if (cmp < 0){
			// go right
			start = mid + 1;
		}else{
			// go left
			end = mid;
		}
	}

	*index = start;
	return cmp;
}

bool VectorList::_shiftL(uint64_t index){
	// this is the most slow operation of them all
	memmove(& _buffer[index], & _buffer[index + 1], (_count - index - 1) * sizeof(void *));

	_resize(-1);

	return true;
}

bool VectorList::_shiftR(uint64_t index){
	if (! _resize(1))
		return false;

	// this is the most slow operation of them all
	memmove(& _buffer[index + 1], & _buffer[index], (_count - index - 1) * sizeof(void *));

	return true;
}

bool VectorList::_resize(int delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	delta = SGN(delta);

	uint64_t new_count = _count + delta;

	if (new_count == 0){
		_clear(true);
		return true;
	}

	size_t new_bufferSize = __calcNewSize(new_count * sizeof(void *), _reallocSize);

	if (_bufferSize == new_bufferSize){
		// already resized, done :)
		_count = new_count;

		return true;
	}

	void *new_buffer = xrealloc(_buffer, new_bufferSize);

	if (new_buffer == NULL)
		return false;

	_count		= new_count;
	_bufferSize	= new_bufferSize;
	_buffer		= (Pair**) new_buffer;

	return true;
}

size_t VectorList::__calcNewSize(size_t size, size_t reallocSize){
	size_t newsize = size / reallocSize;

	if (size % reallocSize)
		newsize++;

	return newsize * reallocSize;
}

// ===================================

inline int SGN(int a){
	if (a == 0)
		return 0;

	return a > 0 ? 1 : -1;
}

