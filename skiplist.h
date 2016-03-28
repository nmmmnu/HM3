#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"

#include <random>


namespace hm3{


class SkipList : public IMutableList<SkipList>{
public:
	using height_type = uint8_t;

public:
	static const height_type MAX_HEIGHT		= 64;
	static const height_type DEFAULT_HEIGHT	= 32;

	class Iterator;

public:
	explicit
	SkipList(height_type height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	~SkipList();

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const = true) const noexcept{
		return dataCount_;
	}

	size_t getSize() const noexcept{
		return dataSize_;
	}

public:
	Iterator begin() const;
	Iterator end() const;

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	void printLanes() const;
	void printLane(height_type lane) const;

private:
	struct		Node;

	height_type	height_;
	Node		**heads_;
	Node		**loc_;

	count_type	dataCount_;
	size_t		dataSize_;

private:
	void clear_();

	const Node *locate_(const StringRef &key, bool complete_evaluation = false) const;

	height_type _getRandomHeight();

private:
	static std::mt19937 rand_;
};

// ==============================

class SkipList::Iterator : public IIterator<Iterator>{
private:
	friend class SkipList;
	Iterator(const Node *node);

public:
	Iterator &operator++();
	const Pair &operator*() const;
	bool operator==(const Iterator &other) const;

private:
	const Node	*node_;
};

// ==============================

inline SkipList::Iterator SkipList::begin() const{
	return Iterator(heads_[0]);
}

inline SkipList::Iterator SkipList::end() const{
	return Iterator(nullptr);
}


} // namespace

#endif
