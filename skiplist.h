#ifndef _SKIP_LIST_LIST_H
#define _SKIP_LIST_LIST_H

#include "ilist.h"

#include <random>

class SkipListIterator;

class SkipList : public IList{
	friend class SkipListIterator;

public:
	static const uint8_t MAX_HEIGHT		= 64;
	static const uint8_t DEFAULT_HEIGHT	= 32;

public:
	explicit SkipList(uint8_t height = DEFAULT_HEIGHT);
	SkipList(SkipList &&other);
	~SkipList() override;

private:
	bool _removeAll() final;

	bool _put(const Pair &pair) final;
	bool _put(Pair &&pair) final;
	Pair _get(const StringRef &key) const final;
	bool _remove(const StringRef &key) final;

	uint64_t _getCount() const noexcept final{
		return _dataCount;
	}

	size_t _getSize() const noexcept final{
		return _dataSize;
	}

	template <typename UPAIR>
	bool _putT(UPAIR &&data);

	std::unique_ptr<IIterator> _getIterator() const final;

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

//	void swap(SkipList &other) noexcept;

private:
	static std::mt19937 __rand;
};

// ==============================

inline bool SkipList::_put(const Pair &data){
	return _putT(data);
}

inline bool SkipList::_put(Pair &&data){
	return _putT(std::move(data));
}


#endif
