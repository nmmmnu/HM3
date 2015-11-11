#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <array>

template <class LIST, size_t CAPACITY>
class HashList : public IList<HashList<LIST, CAPACITY> >{
public:
	typedef std::array<LIST, CAPACITY> container_type;

public:
	HashList() = default;

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	size_t getCount(bool const estimated = false) const noexcept;
	size_t getSize() const noexcept;

public:
	bool put(const Pair &pair);
	bool put(Pair &&pair);

public:
	MultiTableIterator<container_type> begin() const{
		return MultiTableIterator<container_type>(_container);
	}

	MultiTableIterator<container_type> end() const{
		return MultiTableIterator<container_type>(_container, true);
	}

private:
	static unsigned long _calcHash(const char *str);

	size_t _getBucketForKey(const StringRef &key) const{
		/*
		Instead of modulo, this can be done with bit AND.
		Doing so, really increase the speed a bit,
		but code for file based operations get more complicated,
		and overal result will be low quality code.
		*/

		return _calcHash(key.data()) % CAPACITY + 1;
	}

private:
	container_type	_container;
};

// ===================================

#include "hashlist_implementation.h"

#endif

