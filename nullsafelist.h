#ifndef _NULL_SAFE_LIST_H
#define _NULL_SAFE_LIST_H

#include "ilist.h"

class NULLSafeList final : virtual public IList{
public:
	NULLSafeList(IList & list, bool ownList = false) :
		_list(list), _ownList(ownList){}

	virtual ~NULLSafeList() override{
		if (_ownList)
			delete &_list;
	}

	virtual void removeAll() override{
		_list.removeAll();
	}

	virtual bool put(const Pair *pair) override{
		return _list.put(pair);
	}

	virtual const Pair *get(const char *key) const override{
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
	virtual bool rewind() override{
		return _list.rewind();
	}

	virtual bool rewind(const char *key) override{
		return _list.rewind(key);
	}

	virtual const Pair *next() override{
		return _list.next();
	}

	virtual const Pair *current() override{
		return _list.current();
	}

private:
	IList	& _list;
	bool	_ownList;

};

#endif
