#include "iarray.h"

bool IArray::rewind(const char *key){
	if (key){
		uint64_t index;
		if (locatePosition(key, & index))
			return false;

		_itPos = index;
		return true;
	}

	_itPos = 0;
	return true;
}

const Pair *IArray::next(){
	if (_itPos >= getCount())
		return NULL;

	return getAt(_itPos++);
}

