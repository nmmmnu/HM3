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

	count_type getCount(bool const = true) const{
		return dataCount_;
	}

	size_t getSize() const{
		return dataSize_;
	}

public:
	Iterator begin() const;
	Iterator end() const;

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data);

private:
	struct Node;

	Node		*head_;

	count_type	dataCount_;
	size_t		dataSize_;

private:
	void clear_();

	Node *locate_(const StringRef &key) const;
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
	const Node	*node_;
};


}

#endif
