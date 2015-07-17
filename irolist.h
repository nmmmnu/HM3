#ifndef _IROLIST_H
#define _IROLIST_H

#include "iiterator.h"
#include "icountable.h"

class IROList : virtual public IIterator, virtual public ICountable{
public:
	virtual const Pair get(const char *key) const = 0;

	virtual size_t getSize() const = 0;

	inline const Pair operator[](const char *key) const;

	inline bool exists(const char *key) const;

	inline IIterator &getIterator();
};

// ==============================

inline const Pair IROList::operator[](const char *key) const{
	return get(key);
}

inline bool IROList::exists(const char *key) const{
	return (bool) get(key);
}

inline IIterator &IROList::getIterator(){
	return *this;
}

#endif

