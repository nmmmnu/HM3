#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkList : virtual public IList{
public:
	LinkList();
	virtual ~LinkList() override;

	virtual void removeAll() override;

	virtual bool put(const Pair *pair) override;
	virtual const Pair *get(const char *key) const override;
	virtual bool remove(const char *key) override;

	virtual uint64_t getCount() const override;
	virtual size_t getSize() const override;

public:
	virtual bool rewind(const char *key = nullptr) override;

protected:
	virtual const Pair *_next() override;

private:
	struct Node;

	Node		*_head;

	uint64_t	_dataCount;
	size_t		_dataSize;

	const Node	*_itHead;
private:
	void _clear();

	Node *_locate(const char *key) const;
};

#endif
