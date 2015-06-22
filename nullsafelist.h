#ifndef _NULL_SAFE_LIST_H
#define _NULL_SAFE_LIST_H

#include "ilist.h"

class NULLSafeList final : public IList{
public:
	NULLSafeList(IList *list, bool destroyList = true){
		_list = list;
		_deleteList = destroyList;
	}

	virtual ~NULLSafeList(){
		if (_deleteList)
			delete _list;
	}

	virtual void removeAll(){
		_list->removeAll();
	}

	virtual bool put(Pair *pair){
		if (pair == NULL)
			return false;

		return _list->put(pair);
	}

	virtual const Pair *get(const char *key) const{
		if (key == NULL)
			return NULL;

		return _list->get(key);
	}

	virtual bool remove(const char *key){
		if (key == NULL)
			return false;

		return _list->remove(key);
	}

	virtual uint64_t getCount() const{
		return _list->getCount();
	}

	virtual size_t getSize() const{
		return _list->getSize();
	}

public:
	virtual const Pair *first(const char *key = NULL){
		return _list->first(key);
	}

	virtual const Pair *next(){
		return _list->next();
	}

private:
	IList	*_list;
	bool	_deleteList;

};

#endif
