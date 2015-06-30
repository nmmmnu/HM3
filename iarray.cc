#include "iarray.h"

bool IArray::rewind(){
	_itPos = 0;
	return true;
}

bool IArray::rewind(const char *key){
	if (!key)
		return rewind();

	uint64_t index;
	if (lookup(key, index))
		return false;

	_itPos = index;
	return true;
}

const Pair *IArray::forward(){
	if (_itPos >= getCount())
		return nullptr;

	return getAt(_itPos++);
}

// ==============================

const Pair *IArray::get(const char *key) const{
	uint64_t index;
	if (lookup(key, index))
		return nullptr;

	return getAt(index);
}

// ==============================

int IArray::_lookupLinearSearch(const char *key, uint64_t &index) const{
	if (isEmpty()){
		index = 0; return 1;
	}

	int cmp = -1;

	uint64_t i;
	for(i = 0; i < getCount(); ++i){
		auto data = getAt(i);

		cmp = data->cmp(key);

		if (cmp == 0){
			// found
			index = i; return 0;
		}

		if (cmp > 0)
			break;
	}

	index = i; return cmp;
}

int IArray::_lookupBinSearch(const char *key, uint64_t &index) const{
	if (isEmpty()){
		index = 0; return 1;
	}

	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	uint64_t start = 0;
	uint64_t end   = getCount();
	int cmp = 0;

	while (start < end){
	//	uint64_t mid = start + ((end - start) /  2);
		uint64_t mid = start + ((end - start) >> 1);

		auto data = getAt(mid);

		cmp = data->cmp(key);

		if (cmp == 0){
			// found
			index = mid; return 0;
		}

		if (cmp < 0){
			// go right
			start = mid + 1;
		}else{
			// go left
			end = mid;
		}
	}

	index = start; return cmp;
}
