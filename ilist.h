#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "pair.h"



class IListDefs{
public:
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
		return getCount() == 0;
	}

public:
	count_type getCount() const{
		return static_cast<const T*>(this)->getCount();
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

