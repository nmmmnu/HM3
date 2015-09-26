#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkListIterator;

class LinkList : virtual public IList{
friend class LinkListIterator;
public:
	LinkList();
	LinkList(LinkList &&other);
	LinkList &operator = (LinkList &&other);
	~LinkList() override;

private:
	void _removeAll() override;

	bool _put(const Pair &pair) override;
	Pair _get(const StringRef &key) const override;
	bool _remove(const StringRef &key) override;

	count_type _getCount() const override;
	size_t _getSize() const override;

	std::unique_ptr<IIterator> _getIterator() const override;

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
