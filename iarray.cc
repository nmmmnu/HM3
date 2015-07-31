#include "iarray.h"

#include <stdio.h>

Pair IArray::_get(const char *key) const{
	uint64_t index;
	if (lookup(key, index))
		return nullptr;

	return _getAt(index);
}

int IArray::_cmpAt(uint64_t index, const char *key) const{
	return getAt(index).cmp(key);
}

// ==============================

int IArray::_lookupLinearSearch(const char *key, uint64_t &index) const{
	if (isEmpty()){
		index = 0; return 1;
	}

	int cmp = -1;

	uint64_t i;
	for(i = 0; i < getCount(); ++i){
		cmp = _cmpAt(i, key);

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

		cmp = _cmpAt(mid, key);

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

// ==============================

class IArrayIterator : public IIterator{
public:
	IArrayIterator(const IArray & list) :
			_list(list){}

private:
	virtual void _rewind(const char *key = nullptr) override;
	virtual Pair _next() override;
	virtual uint64_t _getVersion() override{
		return _list.getVersion();
	};

private:
	const IArray	& _list;
	uint64_t	_itPos = 0;
};

void IArrayIterator::_rewind(const char *key){
	if (key == nullptr){
		_itPos = 0;
		return;
	}

	uint64_t index;
	_list.lookup(key, index);

	_itPos = index;
}

Pair IArrayIterator::_next(){
	if (_itPos >= _list.getCount())
		return nullptr;

	return _list.getAt(_itPos++);
}

// ==============================

std::unique_ptr<IIterator> IArray::_getIterator() const{
	return std::unique_ptr<IIterator>( new IArrayIterator(*this) );
}
