#include "iarray.h"

int IArray::_lookupLinearSearch(const StringRef &key, uint64_t &index) const{
	uint64_t const count = getCount();

	if (count == 0){
		index = 0; return 1;
	}

	int cmp = -1;

	uint64_t i;
	for(i = 0; i < count; ++i){
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

int IArray::_lookupBinarySearch(const StringRef &key, uint64_t &index) const{
	uint64_t const count = getCount();

	if (count == 0){
		index = 0; return 1;
	}

	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	uint64_t start = 0;
	uint64_t end   = count;
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
	virtual void _rewind(const StringRef &key) override;
	virtual Pair _next() override;
	virtual version_type _getVersion() const override{
		return _list.getVersion();
	};

private:
	const IArray	& _list;
	uint64_t	_itPos = 0;
};

void IArrayIterator::_rewind(const StringRef &key){
	if (key.empty()){
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
	//return std::unique_ptr<IIterator>( new IArrayIterator(*this) );
	return std::make_unique<IArrayIterator>(*this);
}
