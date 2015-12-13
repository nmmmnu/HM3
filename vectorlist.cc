#include "vectorlist.h"

//#define xmalloc(s)	malloc(s)
#define xfree(a)	free(a)
#define xrealloc(a, s)	realloc(a, s)



template<typename T>
inline T SGN(const T a){
	if (a == 0)
		return 0;

	return a > 0 ? 1 : -1;
}



template <class LOOKUP>
VectorList<LOOKUP>::VectorList(size_t const reallocSize) :
		_reallocSize( reallocSize ? reallocSize : REALLOC_SIZE ) {
	_clear();
}

template <class LOOKUP>
VectorList<LOOKUP>::VectorList(VectorList &&other):
		_reallocSize	(std::move(other._reallocSize		)),
		_buffer		(std::move(other._buffer		)),
		_bufferReserved	(std::move(other._bufferReserved	)),
		_dataCount	(std::move(other._dataCount		)),
		_dataSize	(std::move(other._dataSize		)){
	other._clear();
}

template <class LOOKUP>
bool VectorList<LOOKUP>::removeAll(){
	for(count_type i = 0; i < _dataCount; ++i)
		_buffer[i].~Pair();

	_clear(true);

	return true;
}

template <class LOOKUP>
template <class UPAIR>
bool VectorList<LOOKUP>::_putT(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	const auto &l = lookup(key);
	const auto cmp   = std::get<0>(l);
	const auto index = std::get<1>(l);

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
		olddata = std::forward<UPAIR>(newdata);

		return true;
	}

	// key not exists, shift, then add
	if ( ! _shiftR(index) ){
		return false;
	}

	_dataSize += newdata.getSize();

	// placement new with copy constructor
	void *placement = & _buffer[index];
	new(placement) Pair(std::forward<UPAIR>(newdata));

	return true;
}

template <class LOOKUP>
bool VectorList<LOOKUP>::remove(const StringRef &key){
	const auto &l = lookup(key);
	const auto cmp   = std::get<0>(l);
	const auto index = std::get<1>(l);

	if (cmp){
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

template <class LOOKUP>
void VectorList<LOOKUP>::_clear(bool const alsoFree){
	if (alsoFree && _buffer)
		xfree(_buffer);

	_dataCount = 0;
	_dataSize = 0;
	_bufferReserved = 0;
	_buffer = nullptr;
}

template <class LOOKUP>
bool VectorList<LOOKUP>::_shiftL(size_t const index){
	// this is the most slow operation of them all
	memmove(& _buffer[index], & _buffer[index + 1], (_dataCount - index - 1) * ELEMENT_SIZE);

	_resize(-1);

	return true;
}

template <class LOOKUP>
bool VectorList<LOOKUP>::_shiftR(size_t const index){
	if (! _resize(1))
		return false;

	// this is the most slow operation of them all
	memmove(& _buffer[index + 1], & _buffer[index], (_dataCount - index - 1) * ELEMENT_SIZE);

	return true;
}

template <class LOOKUP>
bool VectorList<LOOKUP>::_resize(int const delta){
	if (delta == 0){
		// already resized, done :)
		return true;
	}

	if (_dataCount == 0 && delta < 0){
		// must be an error
		return true;
	}

	count_type const new_dataCount = (count_type) (_dataCount + SGN(delta));

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

template <class LOOKUP>
size_t VectorList<LOOKUP>::__calcNewSize(size_t const size, size_t const reallocSize){
	size_t newsize = size / reallocSize;

	if (size % reallocSize)
		++newsize;

	return newsize * reallocSize;
}

// ===================================

template <class LOOKUP>
VectorList<LOOKUP>::Iterator::Iterator(const VectorList &list, count_type const pos) :
			_list(list),
			_pos(pos){}

template <class LOOKUP>
auto VectorList<LOOKUP>::Iterator::operator++() -> Iterator &{
	++_pos;
	return *this;
}

template <class LOOKUP>
auto VectorList<LOOKUP>::Iterator::operator--() -> Iterator &{
	--_pos;
	return *this;
}

template <class LOOKUP>
const Pair &VectorList<LOOKUP>::Iterator::operator*() const{
	return _list.getAt(_pos);
}

template <class LOOKUP>
bool VectorList<LOOKUP>::Iterator::operator==(const Iterator &other) const{
	return &_list == &other._list && _pos == other._pos;
}

// ===================================

template <class LOOKUP>
auto VectorList<LOOKUP>::begin() const -> Iterator{
	return Iterator(*this, 0);
}

template <class LOOKUP>
auto VectorList<LOOKUP>::end() const -> Iterator{
	return Iterator(*this, getCount());
}

// ===================================

template class VectorList<IArraySearch::Linear>;

template bool VectorList<IArraySearch::Linear>::_putT(Pair &&newdata);
template bool VectorList<IArraySearch::Linear>::_putT(const Pair &newdata);

template class VectorList<IArraySearch::Binary>;

template bool VectorList<IArraySearch::Binary>::_putT(Pair &&newdata);
template bool VectorList<IArraySearch::Binary>::_putT(const Pair &newdata);

