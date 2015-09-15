#ifndef _ILIST_H
#define _ILIST_H

#include "itable.h"

class IList : virtual public ITable{
public:
	void removeAll();
	bool put(const Pair & pair);
	bool remove(const StringRef &key);

private:
	virtual void _removeAll() = 0;

	virtual bool _put(const Pair & pair) = 0;
	virtual bool _remove(const StringRef &key) = 0;

};

// ==============================

inline void IList::removeAll(){
	if (getSize() == 0)
		return;

	incVersion();
	_removeAll();
}

inline bool IList::put(const Pair & pair){
	if (pair){
		incVersion();
		return  _put(pair);
	}
	
	return false;
}

inline bool IList::remove(const StringRef &key){
	if (! key.empty()){		
		incVersion();
		return _remove(key);
	}
	
	return true;
}

#endif

