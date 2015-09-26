#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "ilist.h"

#include <random>

class SkipListIterator;

class SkipList : virtual public IList{
friend class SkipListIterator;
public:
	static const uint8_t MAX_HEIGHT		= 64;
	static const uint8_t DEFAULT_HEIGHT	= 32;

public:
	explicit SkipList(uint8_t height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	SkipList &operator = (SkipList &&other);
	~SkipList() override;

private:
	virtual void _removeAll() override;

	bool _put(const Pair &pair) override;
	Pair _get(const StringRef &key) const override;
	bool _remove(const StringRef &key) override;

	count_type _getCount() const noexcept override{
		return _dataCount;
	}

	size_t _getSize() const noexcept override{
		return _dataSize;
	}

	std::unique_ptr<IIterator> _getIterator() const override;

public:
	void printLanes() const;
	void printLane(uint8_t lane) const;

private:
	struct Node;

	uint8_t		_height;
	Node		**_heads;
	Node		**_loc;

	count_type	_dataCount;
	size_t		_dataSize;

private:
	void _clear();

	const Node *_locate(const StringRef &key, bool complete_evaluation = false) const;

	uint8_t _getRandomHeight();

	void swap(SkipList &other) noexcept;

private:
	static std::mt19937 __rand;
};

#endif
