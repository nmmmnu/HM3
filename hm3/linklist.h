#ifndef _LINK_LIST_LIST_H
#define _LINK_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"

#include <cstdint>


namespace hm3{


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

	size_type getCount(bool const = true) const{
		return dataCount_;
	}

	size_t getSize() const{
		return dataSize_;
	}

public:
	Iterator getIterator(const StringRef &key) const;

	Iterator begin() const;
	Iterator end() const;

private:
	friend class IMutableList<LinkList>;

	template <class UPAIR>
	bool putT_(UPAIR &&data);

private:
	struct Node;

	Node		*head_;

	size_type	dataCount_;
	size_t		dataSize_;

private:
	void clear_();

	Node *locate_(const StringRef &key) const;
};

// ==============================

class LinkList::Iterator : public IIterator<Iterator>{
protected:
	friend class LinkList;

	Iterator(const Node *node) : node_(node){}

public:
	Iterator &operator++();
	const Pair &operator*() const;
	bool operator==(const Iterator &other) const;

private:
	const Node	*node_;
};

// ==============================

inline LinkList::Iterator LinkList::getIterator(const StringRef &key) const{
	const Node *node = locate_(key);
	return Iterator(node);
}

inline LinkList::Iterator LinkList::begin() const{
	return Iterator(head_);
}

inline LinkList::Iterator LinkList::end() const{
	return Iterator(nullptr);
}


}

#endif
