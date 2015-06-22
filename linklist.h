#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

struct LinkListNode;

class LinkList : public IList, public IIterator{
public:
	LinkList();
	virtual ~LinkList();

	virtual void removeAll();

	virtual bool put(Pair *pair);
	virtual const Pair *get(const char *key) const;
	virtual bool remove(const char *key);

	virtual uint64_t getCount() const;
	virtual size_t getSize() const;

	virtual IIterator *getIterator();

public:
	virtual const Pair *first(const char *key = NULL);
	virtual const Pair *next();

private:
	LinkListNode		*_head;

	uint64_t		_dataCount;
	size_t			_dataSize;

	const LinkListNode	*_itHead;
private:
	void _clear();

	LinkListNode *_locate(const char *key) const;

	void _resetIterator();
};

#endif
