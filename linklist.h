#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

struct LinkListNode;

class LinkList : virtual public IList{
public:
	LinkList();
	virtual ~LinkList() override;

	virtual void removeAll() override;

	virtual bool put(Pair *pair) override;
	virtual const OPair get(const char *key) const override;
	virtual bool remove(const char *key) override;

	virtual uint64_t getCount() const override;
	virtual size_t getSize() const override;

public:
	virtual bool rewind(const char *key = NULL) override;
	virtual const OPair next() override;

private:
	LinkListNode		*_head;

	uint64_t		_dataCount;
	size_t			_dataSize;

	const LinkListNode	*_itHead;
private:
	void _clear();

	LinkListNode *_locate(const char *key) const;
};

#endif
