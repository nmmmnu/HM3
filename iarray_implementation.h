#include "iarray.h"

template <class T>
auto IArray<T>::_lookupLinearSearch(const StringRef &key) const -> locator{
	auto const count = getCount();

	if (count == 0){
		// index = 0; return 1;
		return std::make_pair(1, 0);
	}

	int cmp = -1;

	count_type i;
	for(i = 0; i < count; ++i){
		cmp = cmpAt(i, key);

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

template <class T>
auto IArray<T>::_lookupBinarySearch(const StringRef &key) const -> locator{
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

		cmp = cmpAt(mid, key);

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

template <class T>
class IArray<T>::Iterator{
public:
	Iterator(const IArray<T> &list, count_type pos) :
			_list(list),
			_pos(pos){}

	Iterator &operator++(){
		++_pos;
		return *this;
	}

	Iterator &operator--(){
		--_pos;
		return *this;
	}

	Pair operator*() const{
		return _inbound() ? _list.getAt(_pos) : nullptr;
	}

	bool operator==(const Iterator &other) const{
		return &_list == &other._list && _pos == other._pos;
	}

	bool operator!=(const Iterator &other) const{
		return ! ( *this == other );
	}

private:
	bool _inbound() const{
		auto const count = _list.getCount();

		if (count == 0)
			return false;

		if (_pos >= count)
			return false;

		return true;
	}

private:
	const IArray<T>	&_list;
	count_type	_pos;
};

// ==============================

template <class T>
auto IArray<T>::begin() const -> Iterator{
	return Iterator(*this, 0);
}

template <class T>
auto IArray<T>::end() const -> Iterator{
	return Iterator(*this, getCount());
}

