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
	size_type getCount(bool const = true) const noexcept{
		return 0;
	}

	constexpr
	size_t getBytes() const noexcept{
		return 0;
	}

public:
	constexpr
	Iterator lowerBound(const StringRef &) const noexcept{
		return nullptr;
	}

	constexpr
	Iterator begin() const{
		return nullptr;
	}

	constexpr
	Iterator end() const{
		return nullptr;
	}

private:
	friend class IMutableList<BlackHoleList>;

	constexpr
	bool putT_(const Pair &) const {
		return true;
	}

};


} // namespace


#endif
