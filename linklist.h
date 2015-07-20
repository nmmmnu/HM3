#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkList : virtual public IList{
public:
	LinkList();
	LinkList(LinkList &&other);
	virtual ~LinkList() override;

private:
	virtual void _removeAll() override;

	virtual bool _put(const Pair &pair) override;
	virtual Pair _get(const char *key) const override;
	virtual bool _remove(const char *key) override;

	virtual uint64_t _getCount() const override;
	virtual size_t _getSize() const override;

	virtual void _rewind(const char *key = nullptr) override;
	virtual Pair _next() override;

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
