#ifndef _VECTORLIST_H
#define _VECTORLIST_H

#include "arraysearch.h"
#include "iiterator.h"


namespace hm3{


template <class LOOKUP=arraysearch::Binary>
class VectorList : public IMutableList<VectorList<LOOKUP> >{
public:
	using count_type = typename VectorList::count_type;

	static constexpr size_t     ELEMENT_SIZE  = sizeof(Pair);
	static constexpr count_type REALLOC_COUNT = 16;

	class Iterator;

public:
	explicit
	VectorList(count_type reallocCount = REALLOC_COUNT);
	VectorList(VectorList &&other);
	~VectorList(){
		removeAll();
	}

private:
	count_type	reallocCount_;

	Pair		*buffer_;
	count_type	reservedCount_;
	count_type	dataCount_;
	size_t		dataSize_;

	LOOKUP		lookup_;

public:
	bool removeAll();

	bool remove(const StringRef &key);

	const Pair &getAt(count_type const index) const{
		return index < getCount() ? buffer_[index] : Pair::zero();
	}

	int cmpAt(count_type const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	count_type getCount(bool const = false) const{
		return dataCount_;
	}

	size_t getSize() const{
		return dataSize_;
	}

	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return lookup_(*this, key);
	}

	const Pair &get(const StringRef &key) const{
		const auto &l = lookup(key);
		return std::get<0>(l) ? Pair::zero() : getAt( std::get<1>(l) );
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	Iterator begin() const;
	Iterator end() const;

private:
	void clear_(bool alsoFree = false);

	bool shiftL_(count_type index);
	bool shiftR_(count_type index);

	bool resize_(int delta);

	count_type _calcNewCount(count_type size);
};

// ===================================

template <class LOOKUP>
class VectorList<LOOKUP>::Iterator : public IIterator<Iterator>{
private:
	friend class VectorList;
	Iterator(const VectorList *list, count_type pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const VectorList	*list_;
	count_type		pos_;
};


} // namespace


#endif
