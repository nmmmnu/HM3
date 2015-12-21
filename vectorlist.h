#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "iarraysearch.h"
#include "iiterator.h"

template <class LOOKUP=IArraySearch::Binary>
class VectorList : public IMutableList<VectorList<LOOKUP> >{
public:
	using count_type = typename VectorList::count_type;

	static constexpr size_t     ELEMENT_SIZE  = sizeof(Pair);
	static constexpr count_type REALLOC_COUNT = 16;

	class Iterator;

public:
	explicit VectorList(count_type reallocCount = REALLOC_COUNT);
	VectorList(VectorList &&other);
	~VectorList(){
		removeAll();
	}

private:
	count_type	_reallocCount;

	Pair		*_buffer;
	count_type	_reservedCount;
	count_type	_dataCount;
	size_t		_dataSize;

public:
	bool removeAll();

	bool remove(const StringRef &key);

	const Pair &getAt(count_type const index) const{
		return index < getCount() ? _buffer[index] : Pair::zero();
	}

	int cmpAt(count_type const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	count_type getCount(bool const = false) const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

	const Pair &get(const StringRef &key) const{
		const auto &l = lookup(key);
		return std::get<0>(l) ? Pair::zero() : getAt( std::get<1>(l) );
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data);

public:
	Iterator begin() const;
	Iterator end() const;

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(count_type index);
	bool _shiftR(count_type index);

	bool _resize(int delta);

	count_type _calcNewCount(count_type size);
};

// ===================================

template <class LOOKUP>
class VectorList<LOOKUP>::Iterator : public IIterator<Iterator>{
private:
	friend class VectorList;
	Iterator(const VectorList &list, count_type pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const VectorList	&_list;
	count_type		_pos;
};

#endif
