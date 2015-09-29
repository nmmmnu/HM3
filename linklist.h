#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"

class LinkListIterator;

class LinkList : public IList{
	friend class LinkListIterator;

public:
	LinkList();
	LinkList(LinkList &&other);
	~LinkList() override;

private:
	bool _removeAll() final;

	bool _put(const Pair &pair) final;
	bool _put(Pair &&pair) final;
	Pair _get(const StringRef &key) const final;
	bool _remove(const StringRef &key) final;

	uint64_t _getCount() const final{
		return _dataCount;
	}

	size_t _getSize() const final{
		return _dataSize;
	}

	template <typename UPAIR>
	bool _putT(UPAIR &&data);

	std::unique_ptr<IIterator> _getIterator() const final;

private:
	struct Node;

	Node		*_head;

	uint64_t	_dataCount;
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
