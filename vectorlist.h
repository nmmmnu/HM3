#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "iarray.h"
#include "iiterator.h"

class VectorList : public IList<VectorList>, public IArray<VectorList, IArraySearch::Binary>{
public:
	static const size_t ELEMENT_SIZE = sizeof(Pair);
	static const size_t REALLOC_SIZE = 16;

	typedef IListDefs::count_type count_type;

	class Iterator;

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
	int  cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

private:
	const Pair &getAtR(count_type index) const;

public:
	bool put(const Pair &pair){
		return _putT(pair);
	}

	bool put(Pair &&pair){
		return _putT(std::move(pair));
	}

private:
	template <class UPAIR>
	bool _putT(UPAIR &&data);

public:
	Iterator begin() const;
	Iterator end() const;

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

// ===================================

class VectorList::Iterator : public IIterator<VectorList::Iterator>{
private:
	friend class VectorList;
	Iterator(const VectorList &list, count_type pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;
//	bool operator!=(const Iterator &other) const;

private:
	const VectorList	&_list;
	count_type		_pos;
};

// ===================================


inline const Pair &VectorList::getAtR(count_type const index) const{
	static Pair zero;

	return index < getCount() ? _buffer[index] : zero;
}

inline Pair VectorList::getAt(count_type index) const{
	return getAtR(index);
}

inline int  VectorList::cmpAt(count_type const index, const StringRef &key) const{
	return getAtR(index).cmp(key);
}

#endif
