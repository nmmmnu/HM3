#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include <cstdint>

#include "list.h"

class LinkList : public IList{
public:
	LinkList();
	LinkList(LinkList &&other);
	~LinkList();

public:
	bool removeAll();

	Pair get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount() const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

public:
	bool put(const Pair &pair){
		return _putT(pair);
	}

	bool put(Pair &&pair){
		return _putT(std::move(pair));
	}

private:
	template <typename UPAIR>
	bool _putT(UPAIR &&data);

private:
	struct Node;

	Node		*_head;

	count_type	_dataCount;
	size_t		_dataSize;

private:
	void _clear();

	Node *_locate(const StringRef &key) const;
};

// ==============================

#endif
