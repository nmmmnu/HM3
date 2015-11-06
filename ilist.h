#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "pair.h"



struct IListDefs{
	typedef uint32_t count_type;

};

// ==============================

template <class T>
class IList : public IListDefs{
protected:
	constexpr
	static count_type PRINT_COUNT = 10;

public:
	void print(count_type count = PRINT_COUNT) const;

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
void IList<T>::print(count_type const count) const{
	auto self = static_cast<const T*>(this);

	count_type i = 0;

	for(const Pair &p : *self){
		p.print();
		if (++i > count)
			return;
	}
}

#endif

