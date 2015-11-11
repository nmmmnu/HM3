#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "iarraysearch.h"
#include "iiterator.h"

template <class LOOKUP=IArraySearch::Binary>
class VectorList : public IList<VectorList<LOOKUP> >{
public:
	static const size_t ELEMENT_SIZE = sizeof(Pair);
	static const size_t REALLOC_SIZE = 16;

	class Iterator;

public:
	explicit VectorList(size_t reallocSize = 0);
	VectorList(VectorList &&other);
	~VectorList(){
		removeAll();
	}

private:
	size_t		_reallocSize;

	Pair		*_buffer;
	size_t		_bufferReserved;

	uint64_t	_dataCount;
	size_t		_dataSize;

public:
	bool removeAll();

	bool remove(const StringRef &key);

	const Pair &getAt(size_t const index) const{
		return index < getCount() ? _buffer[index] : ListDefs::zero;
	}

	int cmpAt(size_t const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	size_t getCount(bool const = false) const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

	std::tuple<int, size_t> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

	const Pair &get(const StringRef &key) const{
		const auto &l = lookup(key);
		return std::get<0>(l) ? ListDefs::zero : getAt( std::get<1>(l) );
	}

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

template <class LOOKUP>
class VectorList<LOOKUP>::Iterator : public IIterator<Iterator>{
private:
	friend class VectorList;
	Iterator(const VectorList &list, size_t pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const VectorList	&_list;
	size_t			_pos;
};

#endif
