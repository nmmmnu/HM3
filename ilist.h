#ifndef _ILIST_H
#define _ILIST_H

#include "pair.h"
#include "iiterator.h"
#include "icountable.h"

class IList : virtual public IIterator, virtual public ICountable{
public:
	virtual ~IList(){};

	virtual void removeAll() = 0;

	virtual bool put(Pair *pair) = 0;
	virtual const Pair *get(const char *key) const = 0;
	virtual bool remove(const char *key) = 0;

	virtual uint64_t getSize() const = 0;

	inline bool exists(const char *key) const;

	inline IIterator *getIterator();
};

// ==============================

inline bool IList::exists(const char *key) const{
	return get(key) != NULL;
}

inline IIterator *IList::getIterator(){
	return this;
}

#endif

