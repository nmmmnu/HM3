#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "ilist.h"
#include "iarray.h"

class VectorList : virtual public IList, virtual public IArray{
public:
	static const size_t ELEMENT_SIZE = sizeof(Pair);
	static const size_t REALLOC_SIZE = 16;

public:
	explicit VectorList(size_t reallocSize = 0);
	VectorList(VectorList &&other);
	VectorList &operator = (VectorList &&other);
	~VectorList() override;

private:
	size_t		_reallocSize;

	Pair		*_buffer;
	size_t		_bufferReserved;

	count_type	_dataCount;
	size_t		_dataSize;

private:
	void _removeAll() override;

	bool _put(const Pair &data) override;
	bool _put(Pair &&data) override;
	bool _remove(const StringRef &key) override;

	Pair _getAt(uint64_t index) const override;
	int  _cmpAt(uint64_t index, const StringRef &key) const override;

	count_type _getCount() const override;
	size_t _getSize() const override;

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

	template <typename UPAIR>
	bool _putT(UPAIR &&data);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

// ===================================

inline Pair VectorList::_getAt(uint64_t const index) const{
	return index < getCount() ? _buffer[index] : nullptr;
}

inline int VectorList::_cmpAt(uint64_t const index, const StringRef &key) const{
	return _getAt(index).cmp(key);
}

inline VectorList::count_type VectorList::_getCount() const{
	return _dataCount;
}

inline size_t VectorList::_getSize() const{
	return _dataSize;
}

#endif
