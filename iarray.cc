#include "iarray.h"

IArray::locator IArray::_lookupLinearSearch(const StringRef &key) const{
	auto const count = getCount();

	if (count == 0){
		// index = 0; return 1;
		return std::make_pair(1, 0);
	}

	int cmp = -1;

	count_type i;
	for(i = 0; i < count; ++i){
		cmp = _cmpAt(i, key);

		if (cmp == 0){
			// found
			// index = i; return 0;
			return std::make_pair(0, i);
		}

		if (cmp > 0)
			break;
	}

	// index = i; return cmp;
	return std::make_pair(cmp, i);
}

IArray::locator IArray::_lookupBinarySearch(const StringRef &key) const{
	auto const count = getCount();

	if (count == 0){
		// index = 0; return 1;
		return std::make_pair(1, 0);
	}

	/*
	 * Lazy based from Linux kernel...
	 * http://lxr.free-electrons.com/source/lib/bsearch.c
	 */

	count_type start = 0;
	count_type end   = count;
	int cmp = 0;

	while (start < end){
	//	count_type mid = start + ((end - start) /  2);
		count_type mid = start + ((end - start) >> 1);

		cmp = _cmpAt(mid, key);

		if (cmp == 0){
			// found
			// index = mid; return 0;
			return std::make_pair(0, mid);
		}

		if (cmp < 0){
			// go right
			start = mid + 1;
		}else{
			// go left
			end = mid;
		}
	}

	// index = start; return cmp;
	return std::make_pair(cmp, start);
}

// ==============================

class IArrayIterator : public IIterator{
public:
	IArrayIterator(const IArray &list) :
			IIterator(list),
			_list(list){}

private:
	virtual void _rewind(const StringRef &key) final;
	virtual Pair _next() final;

private:
	const IArray	& _list;
	uint64_t	_itPos = 0;
};

void IArrayIterator::_rewind(const StringRef &key){
	if (key.empty()){
		_itPos = 0;
		return;
	}

	const auto l = _list.lookup(key);

	_itPos = l.second;
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
