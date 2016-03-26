#ifndef _STL_VECTOR_LIST_H
#define _STL_VECTOR_LIST_H

#include "arraysearch.h"

#include <vector>


namespace hm3{


template <class LOOKUP>
class STLVectorList : public IMutableList<STLVectorList<LOOKUP> >{
public:
	using vector_type = std::vector<Pair>;

	using count_type  = vector_type::size_type;
	using Iterator    = vector_type::const_iterator;

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

	LOOKUP		lookup_;

public:
	bool removeAll(){
		container_.clear();
		dataSize_ = 0;

		return true;
	}

	bool remove(const StringRef &key);

	const Pair &getAt(count_type const index) const{
		return index < getCount() ? container_[index] : Pair::zero();
	}

	int cmpAt(count_type const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	count_type getCount(bool const = false) const{
		return container_.size();
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
	Iterator begin() const{
		return container_.cbegin();
	}

	Iterator end() const{
		return container_.cend();
	}
};


} // namespace


#endif
