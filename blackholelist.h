#ifndef _BLACK_HOLE_LIST_LIST_H
#define _BLACK_HOLE_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"


namespace hm3{


class BlackHoleList : public IMutableList<BlackHoleList>{
public:
	using Iterator = EmptyIterator;

public:
	BlackHoleList() = default;

public:
	constexpr
	bool removeAll() const noexcept{
		return true;
	}

	constexpr
	const Pair &get(const StringRef &) const noexcept{
		return Pair::zero();
	}

	constexpr
	bool remove(const StringRef &key) const noexcept{
		return true;
	}

	constexpr
	count_type getCount(bool const = true) const noexcept{
		return 0;
	}

	constexpr
	size_t getSize() const noexcept{
		return 0;
	}

public:
	Iterator begin() const{
		return nullptr;
	}

	Iterator end() const{
		return nullptr;
	}

public:
	// needs to be public because of CRPT
	// no need to be templated as well
	bool putT_(...) const {
		return true;
	}

};


} // namespace


#endif
