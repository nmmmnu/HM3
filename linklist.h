#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkListIterator;

class LinkList : virtual public IList{
friend class LinkListIterator;
public:
	LinkList();
	LinkList(LinkList &&other);
	~LinkList() override;

private:
	void _removeAll() override;

	bool _put(const Pair &pair) override;
	bool _put(Pair &&pair) override;
	Pair _get(const StringRef &key) const override;
	bool _remove(const StringRef &key) override;

	count_type _getCount() const override{
		return _dataCount;
	}

	size_t _getSize() const override{
		return _dataSize;
	}

	template <typename UPAIR>
	bool _putT(UPAIR &&data);

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

inline bool LinkList::_put(const Pair &data){
	return _putT(data);
}

inline bool LinkList::_put(Pair &&data){
	return _putT(std::move(data));
}

#endif
