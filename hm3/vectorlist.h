#ifndef _VECTORLIST_H
#define _VECTORLIST_H

#include "arraysearch/binary.h"
#include "arraysearch/linear.h"
#include "iiterator.h"
#include "ilist.h"

namespace hm3{


template <class LOCATOR=arraysearch::Binary>
class VectorList : public IMutableList<VectorList<LOCATOR> >{
private:
	using ArraySearch	= arraysearch::SimpleSearch<LOCATOR>;

public:
	using count_type = typename VectorList::count_type;

	static constexpr size_t     ELEMENT_SIZE  = sizeof(Pair);
	static constexpr count_type REALLOC_COUNT = 16;

	using Iterator = const Pair *;

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

	const Pair &get(const StringRef &key) const{
		const auto &lr = lookup(key);
		return lr ? getAt( lr.get() ) : Pair::zero();
	}

private:
	friend class IMutableList<VectorList<LOCATOR> >;

	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	Iterator getIterator(const StringRef &key) const noexcept{
		const auto &lr = lookup(key);
		return buffer_ + lr.get();
	}

	Iterator begin() const noexcept{
		return buffer_;
	}

	Iterator end() const noexcept{
		return buffer_ + dataCount_;
	}

private:
	void clear_(bool alsoFree = false);

	bool shiftL_(count_type index);
	bool shiftR_(count_type index);

	bool resize_(int delta);

	count_type calcNewCount_(count_type size);

private:
	ArraySearch	search_;

	auto lookup(const StringRef &key) const -> decltype( search_.operator()(*this, key) ){
		return search_(*this, key);
	//	return LOCATOR_(*this, key);
	}

};

} // namespace


#endif
