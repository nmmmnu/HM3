#ifndef _IITERATOR_H
#define _IITERATOR_H

template <class T>
class IIterator{
public:
	bool operator!=(const IIterator &other) const{
		const T *a = static_cast<const T*>(this);
		const T *b = static_cast<const T*>(&other);

		return ! ( *a == *b );
	}

	const Pair *operator->() const{
		const Pair &p = static_cast<const T*>(this)->operator*();

		return &p;
	}
};

using EmptyIterator = const Pair *;

#endif

