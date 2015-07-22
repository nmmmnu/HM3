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
	SkipList(uint8_t height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	virtual ~SkipList() override;

private:
	virtual void _removeAll() override;

	virtual bool _put(const Pair &pair) override;
	virtual Pair _get(const char *key) const override;
	virtual bool _remove(const char *key) override;

	virtual uint64_t _getCount() const override;
	virtual size_t _getSize() const override;

	virtual std::unique_ptr<IIterator> _getIterator() const override;

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

	const Node *_locate(const char *key, bool complete_evaluation = false) const;

	uint8_t _getRandomHeight();

private:
	static std::mt19937 __rand;
};

#endif
