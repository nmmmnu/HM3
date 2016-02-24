#ifndef _BLACK_HOLE_LIST_LIST_H
#define _BLACK_HOLE_LIST_LIST_H

#include "ilist.h"
#include "emptyiterator.h"

class BlackHoleList : public IMutableList<BlackHoleList>{
public:
	using Iterator = EmptyIterator<BlackHoleList>;

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
		return Iterator(this);
	}

	Iterator end() const{
		return Iterator(this);
	}

public:
	// needs to be public because of CRPT
	// no need to be templated as well
	bool _putT(...) const {
		return true;
	}

};

#endif
