#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "list.h"

#include <random>

class SkipListIterator;

class SkipList : public IList{
public:
	static const uint8_t MAX_HEIGHT		= 64;
	static const uint8_t DEFAULT_HEIGHT	= 32;

public:
	explicit SkipList(uint8_t height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	~SkipList();

public:
	bool removeAll();

	Pair get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount() const noexcept{
		return _dataCount;
	}

	size_t getSize() const noexcept{
		return _dataSize;
	}

public:
	bool put(const Pair &pair){
		return _putT(pair);
	}

	bool put(Pair &&pair){
		return _putT(std::move(pair));
	}

private:
	template <typename UPAIR>
	bool _putT(UPAIR &&data);

public:
	void printLanes() const;
	void printLane(uint8_t lane) const;

private:
	struct Node;

	uint8_t		_height;
	Node		**_heads;
	Node		**_loc;

	uint64_t	_dataCount;
	size_t		_dataSize;

private:
	void _clear();

	const Node *_locate(const StringRef &key, bool complete_evaluation = false) const;

	uint8_t _getRandomHeight();

private:
	static std::mt19937 __rand;
};

#endif
