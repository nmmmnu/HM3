#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "iarray.h"

class VectorList : public IArray<VectorList>{
public:
	static const size_t ELEMENT_SIZE = sizeof(Pair);
	static const size_t REALLOC_SIZE = 16;

public:
	explicit VectorList(size_t reallocSize = 0);
	VectorList(VectorList &&other);
	~VectorList();

private:
	size_t		_reallocSize;

	Pair		*_buffer;
	size_t		_bufferReserved;

	uint64_t	_dataCount;
	size_t		_dataSize;

public:
	bool removeAll();

	bool remove(const StringRef &key);

	Pair getAt(count_type index) const;

	const Pair &getAtR(count_type index) const;
	int  cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

public:
	bool put(const Pair &pair){
		return _putT(pair);
	}

	bool put(Pair &&pair){
		return _putT(std::move(pair));
	}

private:
	template <typename UPAIR>
	bool _putT(UPAIR &&data);

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

// ===================================

inline const Pair &VectorList::getAtR(count_type const index) const{
	static Pair zero;

	return index < getCount() ? _buffer[index] : zero;
}

inline int  VectorList::cmpAt(count_type const index, const StringRef &key) const{
	return index < getCount() ? _buffer[index].cmp(key) : +1;
}

inline Pair VectorList::getAt(count_type index) const{
	return getAtR(index);
}

#endif
