#ifndef _ILIST_COLLECTION_H
#define _ILIST_COLLECTION_H

#include "irolist.h"

class IListCollection : public ICountable{
public:
	virtual ~IListCollection(){};

	const IROList & getAt(uint64_t index) const;

	const IROList & operator[](uint64_t index) const;

private:
	virtual const IROList & _getAt(uint64_t index) const = 0;
};

// ==============================

inline const IROList & IListCollection::getAt(uint64_t index) const{
	return _getAt( std::min(index, getCount()) );
}

inline const IROList & IListCollection::operator[](uint64_t index) const{
	return getAt(index);
}

#endif

