#ifndef _ILIST_H
#define _ILIST_H

#include "itable.h"

class IList : virtual public ITable{
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
	incVersion();
	_removeAll();
}

inline bool IList::put(const Pair & pair){
	incVersion();
	return pair ? _put(pair) : false;
}

inline bool IList::remove(const char *key){
	incVersion();
	return key ? _remove(key) : true;
}

#endif

