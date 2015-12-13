#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "pair.h"

// ==============================

namespace ListDefs{
	using count_type = uint64_t;

	extern const Pair zero;
};

// ==============================

template <class T>
class IList{
protected:
	constexpr
	static size_t PRINT_COUNT = 10;

public:
	using count_type = ListDefs::count_type;

public:
	void print(size_t count = PRINT_COUNT) const;

	bool isEmpty() const{
		return getCount(true) == 0;
	}

public:
	count_type getCount(bool const estimated = false) const{
		return static_cast<const T*>(this)->getCount(estimated);
	}
};

// ==============================

template <class T>
void IList<T>::print(size_t count) const{
	auto self = static_cast<const T*>(this);

	for(const Pair &p : *self){
		p.print();
		if (++count == 0)
			return;
	}
}

#endif

