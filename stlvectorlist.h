#ifndef _STL_VECTOR_LIST_H
#define _STL_VECTOR_LIST_H

#include "iarraysearch.h"

#include <vector>

template <class LOOKUP=IArraySearch::Binary>
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
	vector_type	_container;
	size_t		_dataSize = 0;

public:
	bool removeAll(){
		_container.clear();
		_dataSize = 0;

		return true;
	}

	bool remove(const StringRef &key);

	const Pair &getAt(count_type const index) const{
		return index < getCount() ? _container[index] : Pair::zero();
	}

	int cmpAt(count_type const index, const StringRef &key) const{
		return getAt(index).cmp(key);
	}

	count_type getCount(bool const = false) const{
		return _container.size();
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
	Iterator begin() const{
		return _container.cbegin();
	}

	Iterator end() const{
		return _container.cend();
	}
};

#endif
