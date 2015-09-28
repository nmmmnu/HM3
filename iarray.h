#ifndef _IARRAY_H
#define _IARRAY_H

#include "itable.h"

template <typename T>
class IArray : public ITable<IArray<T>>{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BINARY_SEARCH	= 1;

public:
	typedef uint64_t count_type;

public:
	Pair getAt(count_type index) const;
	Pair operator[](count_type index) const;
	int lookup(const StringRef &key, count_type &index) const;

	void setLookupMethod(uint8_t lookupMethod);

private:
	Pair _getAt(count_type const index) const{
		return impl()._getAt(index);
	};

	int _cmpAt(count_type const index, const StringRef &key) const{
		return impl()._cmpAt(index, key);
	};

private:
	Pair _get(const StringRef &key) const;

	std::unique_ptr<IIterator> _getIterator() const;

private:
	uint8_t		_lookupMethod = BINARY_SEARCH;

private:
	int _lookupBinarySearch(const StringRef &key, count_type &index) const;
	int _lookupLinearSearch(const StringRef &key, count_type &index) const;

protected:
	int _cmpAtNaive(count_type index, const StringRef &key) const;

private:
	const T &impl() const{
		return *( static_cast<const T*>(this) );
	}
};

// ==============================

template <typename T>
inline Pair IArray<T>::getAt(count_type const index) const{
	return index < this->getCount() ? _getAt(index) : nullptr;
}

template <typename T>
inline Pair IArray<T>::operator[](count_type const index) const{
	return getAt(index);
}

template <typename T>
inline void IArray<T>::setLookupMethod(uint8_t const lookupMethod){
	_lookupMethod = lookupMethod;
}

template <typename T>
inline int IArray<T>::lookup(const StringRef &key, count_type &index) const{
	// until there are only 2 methods
	if (_lookupMethod == LINEAR_SEARCH)
		return _lookupLinearSearch(key, index);
	else
		return _lookupBinarySearch(key, index);
}

// ==============================

template <typename T>
inline int IArray<T>::_cmpAtNaive(count_type const index, const StringRef &key) const{
	// this will do copy
	return getAt(index).cmp(key);
}

template <typename T>
inline Pair IArray<T>::_get(const StringRef &key) const{
	count_type index;
	return lookup(key, index) ? nullptr : _getAt(index);
}

#endif

