#ifndef _ILIST_COLLECTION_H
#define _ILIST_COLLECTION_H

#include "itable.h"

#include <stdexcept>

class IListCollection : public ICountable{
public:
	virtual ~IListCollection() = default;

	const ITable & getAt(uint64_t index) const;

	const ITable & operator[](uint64_t index) const;

private:
	virtual const ITable & _getAt(uint64_t index) const = 0;
};

// ==============================

inline const ITable & IListCollection::getAt(uint64_t const index) const{
	// if we check getCount() here, there will be virtual dispatch
	return _getAt(index);
}

inline const ITable & IListCollection::operator[](uint64_t const index) const{
	return getAt(index);
}

#endif

