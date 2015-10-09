#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include <cstdint>

#include "ilist.h"

class LinkList : public IList<LinkList>{
public:
	class Iterator;

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

public:
	Iterator begin() const;
	Iterator end() const;

private:
	template <class UPAIR>
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

class LinkList::Iterator{
protected:
	friend class LinkList;

	Iterator(const Node *node);

public:
	Iterator &operator++();
	const Pair &operator*() const;
	bool operator==(const Iterator &other) const;
	bool operator!=(const Iterator &other) const;

private:
	const Node	*_node;

	static Pair	zero;
};

#endif
