#ifndef _EMPTY_IITERATOR_H
#define _EMPTY_IITERATOR_H

#include "iiterator.h"

template <class LIST>
class EmptyIterator : public IIterator<EmptyIterator<LIST> >{
private:
	const LIST	*_list;

private:
	friend LIST;

	constexpr
	EmptyIterator(const LIST *list) : _list(list){
	}

public:
	EmptyIterator &operator++(){
		return *this;
	}

	constexpr
	const Pair &operator*() const{
		return Pair::zero();
	}

	constexpr
	bool operator==(const EmptyIterator &other) const{
		return _list == other._list;
	}
};

#endif

