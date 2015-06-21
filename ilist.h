#ifndef _ILIST_H
#define _ILIST_H

#include "pair.h"
#include "iiterator.h"

class IList{
public:
	virtual ~IList(){};

	virtual void removeAll() = 0;

	virtual bool put(Pair *pair) = 0;
	virtual const Pair *get(const char *key) const = 0;
	virtual bool remove(const char *key) = 0;

	virtual uint64_t getCount() const = 0;
	virtual uint64_t getSize() const = 0;

	inline bool empty() const;
	inline bool exists(const char *key) const;

	virtual IIterator *getIterator() = 0;
};

// ==============================

inline bool IList::empty() const{
	return getCount() == 0;
}

inline bool IList::exists(const char *key) const{
	return get(key) != NULL;
}

#endif

