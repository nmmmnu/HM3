#ifndef _IITERATOR_H
#define _IITERATOR_H

#include "pair.h"

namespace hm3{


template <class T>
class IIterator{
public:
#if 0
	bool operator!=(const IIterator &other) const{
		const T *a = static_cast<const T*>(this);
		const T *b = static_cast<const T*>(&other);

		return ! ( *a == *b );
	}
#endif

	bool operator!=(const T &other) const{
		const T *self = static_cast<const T*>(this);

		return ! ( *self == other );
	}

	const Pair *operator->() const{
		const Pair &p = static_cast<const T*>(this)->operator*();

		return &p;
	}
};


using EmptyIterator = const Pair *;


} // namespace


#endif

