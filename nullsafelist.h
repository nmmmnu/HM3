#ifndef _NULL_SAFE_LIST_H
#define _NULL_SAFE_LIST_H

#include "ilist.h"

class NULLSafeList final : virtual public IList{
public:
	NULLSafeList(IList & list, bool deleteList = false) : _list(list), _deleteList(deleteList){}

	virtual ~NULLSafeList() override{
		if (_deleteList)
			delete &_list;
	}

	virtual void removeAll() override{
		_list.removeAll();
	}

	virtual bool put(Pair *pair) override{
		if (pair == nullptr)
			return false;

		return _list.put(pair);
	}

	virtual OPair get(const char *key) const override{
		if (key == nullptr)
			return nullptr;

		return _list.get(key);
	}

	virtual bool remove(const char *key) override{
		if (key == nullptr)
			return false;

		return _list.remove(key);
	}

	virtual uint64_t getCount() const override{
		return _list.getCount();
	}

	virtual size_t getSize() const override{
		return _list.getSize();
	}

public:
	virtual bool rewind(const char *key = nullptr) override{
		return _list.rewind(key);
	}

	virtual OPair next() override{
		return _list.next();
	}

private:
	IList	& _list;
	bool	_deleteList;

};

#endif
