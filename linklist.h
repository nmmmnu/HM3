#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"

#include <cstdint>

class LinkList : public IMutableList<LinkList>{
public:
	class Iterator;

public:
	LinkList();
	LinkList(LinkList &&other);
	~LinkList(){
		removeAll();
	}

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const = true) const{
		return _dataCount;
	}

	size_t getSize() const{
		return _dataSize;
	}

public:
	Iterator begin() const;
	Iterator end() const;

public:
	// needs to be public because of CRPT
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

class LinkList::Iterator : public IIterator<Iterator>{
protected:
	friend class LinkList;

	Iterator(const Node *node);

public:
	Iterator &operator++();
	const Pair &operator*() const;
	bool operator==(const Iterator &other) const;

private:
	const Node	*_node;
};

#endif
