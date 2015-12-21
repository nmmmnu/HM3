#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "ilist.h"
#include "iiterator.h"

#include <random>

class SkipList : public IMutableList<SkipList>{
public:
	using height_type = uint8_t;

public:
	static const height_type MAX_HEIGHT		= 64;
	static const height_type DEFAULT_HEIGHT	= 32;

	class Iterator;

public:
	explicit SkipList(height_type height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	~SkipList();

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const = true) const noexcept{
		return _dataCount;
	}

	size_t getSize() const noexcept{
		return _dataSize;
	}

public:
	Iterator begin() const;
	Iterator end() const;

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data);

public:
	void printLanes() const;
	void printLane(height_type lane) const;

private:
	struct		Node;

	height_type	_height;
	Node		**_heads;
	Node		**_loc;

	count_type	_dataCount;
	size_t		_dataSize;

private:
	void _clear();

	const Node *_locate(const StringRef &key, bool complete_evaluation = false) const;

	height_type _getRandomHeight();

private:
	static std::mt19937 __rand;
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
	const Node	*_node;
};

#endif
