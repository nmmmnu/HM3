#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "ilist.h"

struct SkipListNode;

class SkipList : virtual public IList{
public:
	static const uint8_t MAX_HEIGHT		= 64;
	static const uint8_t DEFAULT_HEIGHT	= 32;

public:
	SkipList(uint8_t height = DEFAULT_HEIGHT);
	virtual ~SkipList() override;

	virtual void removeAll() override;

	virtual bool put(IPair &pair) override;
	virtual const void *get(const char *key) const override;
	virtual bool remove(const char *key) override;

	virtual uint64_t getCount() const override;
	virtual size_t getSize() const override;

public:
	virtual bool rewind() override;
	virtual bool rewind(const char *key) override;
	virtual const void *next() override;

public:
	void printLanes() const;
	void printLane(uint8_t lane) const;

private:
	uint8_t		_height;
	SkipListNode	**_heads;
	SkipListNode	**_loc;

	uint64_t	_dataCount;
	size_t		_dataSize;

	const SkipListNode	*_itHead;

private:
	void _clear();

	const SkipListNode *_locate(const char *key, bool complete_evaluation = false) const;

	uint8_t _getRandomHeight();
};

#endif
