#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkListIterator;

class LinkList : virtual public IList{
friend class LinkListIterator;
public:
	LinkList();
	LinkList(LinkList &&other);
	virtual ~LinkList() override;

private:
	virtual void _removeAll() override;

	virtual bool _put(const Pair &pair) override;
	virtual Pair _get(const StringRef &key) const override;
	virtual bool _remove(const StringRef &key) override;

	virtual count_type _getCount() const override;
	virtual size_t _getSize() const override;

	virtual std::unique_ptr<IIterator> _getIterator() const override;

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

inline LinkList::count_type LinkList::_getCount() const{
	return _dataCount;
}

inline size_t LinkList::_getSize() const{
	return _dataSize;
}

#endif
