#ifndef _BLACK_HOLE_LIST_LIST_H
#define _BLACK_HOLE_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"

class BlackHoleList : public IMutableList<BlackHoleList>{
public:
	class Iterator;

public:
	BlackHoleList() = default;

public:
	constexpr
	bool removeAll() noexcept{
		return true;
	}

	constexpr
	const Pair &get(const StringRef &) const noexcept{
		return Pair::zero();
	}

	constexpr
	bool remove(const StringRef &key) noexcept{
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
	Iterator begin() const;

	Iterator end() const;

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&){
		return true;
	}

};

// ==============================

class BlackHoleList::Iterator : public IIterator<Iterator>{
private:
	friend class BlackHoleList;

	constexpr
	Iterator(const BlackHoleList *list) : _list(list){
	}

public:
	Iterator &operator++(){
		return *this;
	}

	constexpr
	const Pair &operator*() const{
		return Pair::zero();
	}

	constexpr
	bool operator==(const Iterator &other) const{
		return _list == other._list;
	}

private:
	const BlackHoleList	*_list;
};

// ==============================

inline auto BlackHoleList::begin() const -> Iterator{
	return end();
}

inline auto BlackHoleList::end() const -> Iterator{
	return Iterator(this);
}

#endif
