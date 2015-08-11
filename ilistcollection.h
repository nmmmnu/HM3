#ifndef _ILIST_COLLECTION_H
#define _ILIST_COLLECTION_H

#include "itable.h"

class IListCollection : public ICountable{
public:
	virtual ~IListCollection(){};

	const ITable & getAt(uint64_t index) const;

	const ITable & operator[](uint64_t index) const;

private:
	virtual const ITable & _getAt(uint64_t index) const = 0;
};

// ==============================

inline const ITable & IListCollection::getAt(uint64_t const index) const{
	return _getAt( std::min(index, getCount()) );
}

inline const ITable & IListCollection::operator[](uint64_t const index) const{
	return getAt(index);
}

#endif

