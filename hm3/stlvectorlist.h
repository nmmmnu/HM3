#ifndef _STL_VECTOR_LIST_H
#define _STL_VECTOR_LIST_H

#include "ilist.h"

#include <vector>

namespace hm3{


class STLVectorList : public IMutableList<STLVectorList>{
public:
	using vector_type	= std::vector<Pair>;

	using size_type	= vector_type::size_type;
	using difference_type	= vector_type::difference_type;
	using Iterator		= vector_type::const_iterator;

public:
	STLVectorList() = default;

	// we allow to move
	STLVectorList(STLVectorList &&other) = default;
	STLVectorList &operator =(STLVectorList &&other) = default;

	// but we do not allow copy
	STLVectorList(const STLVectorList &other) = delete;
	STLVectorList &operator =(const STLVectorList &other) = delete;

private:
	vector_type	container_;
	size_t		dataSize_ = 0;

public:
	bool removeAll(){
		container_.clear();
		dataSize_ = 0;

		return true;
	}

	bool remove(const StringRef &key);

	const Pair &getAt(size_type const index) const{
		return index < getCount() ? container_[index] : Pair::zero();
	}

	int cmpAt(size_type const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	size_type getCount(bool const = false) const{
		return container_.size();
	}

	size_t getSize() const{
		return dataSize_;
	}

	const Pair &get(const StringRef &key) const;

private:
	friend class IMutableList<STLVectorList>;

	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	Iterator lowerBound(const StringRef &key) const;

	Iterator begin() const{
		return container_.cbegin();
	}

	Iterator end() const{
		return container_.cend();
	}

private:
	bool binarySearch_(const StringRef &key, size_type &result) const;
};


} // namespace


#endif
