#ifndef _ILIST_H
#define _ILIST_H

#include "irolist.h"

class IList : virtual public IROList{
public:
	void removeAll();
	bool put(const Pair & pair);
	bool remove(const char *key);

private:
	virtual void _removeAll() = 0;

	virtual bool _put(const Pair & pair) = 0;
	virtual bool _remove(const char *key) = 0;
};

// ==============================

inline void IList::removeAll(){
	invalidate();
	_removeAll();
}

inline bool IList::put(const Pair & pair){
	invalidate();
	return pair ? _put(pair) : false;
}

inline bool IList::remove(const char *key){
	invalidate();
	return key ? _remove(key) : true;
}

#endif

