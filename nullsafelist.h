#ifndef _NULL_SAFE_LIST_H
#define _NULL_SAFE_LIST_H

#include "ilist.h"

class NULLSafeList final : virtual public IList{
public:
	NULLSafeList(IList *list, bool destroyList = true){
		_list = list;
		_deleteList = destroyList;
	}

	virtual ~NULLSafeList(){
		if (_deleteList)
			delete _list;
	}

	virtual void removeAll() override{
		_list->removeAll();
	}

	virtual bool put(Pair *pair) override{
		if (pair == NULL)
			return false;

		return _list->put(pair);
	}

	virtual const Pair *get(const char *key) const override{
		if (key == NULL)
			return NULL;

		return _list->get(key);
	}

	virtual bool remove(const char *key) override{
		if (key == NULL)
			return false;

		return _list->remove(key);
	}

	virtual uint64_t getCount() const override{
		return _list->getCount();
	}

	virtual size_t getSize() const override{
		return _list->getSize();
	}

public:
	virtual bool rewind(const char *key = NULL) override{
		return _list->rewind(key);
	}

	virtual const Pair *next() override{
		return _list->next();
	}

private:
	IList	*_list;
	bool	_deleteList;

};

#endif
