#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <array>

template <class LIST, size_t CAPACITY>
class HashList : public IList<HashList<LIST, CAPACITY> >{
public:
	typedef IListDefs::count_type count_type;
//	typedef capacity_type uint16_t;

public:
	HashList() = default;

public:
	bool removeAll();

	Pair get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const estimated = false) const noexcept;
	size_t getSize() const noexcept;

public:
	bool put(const Pair &pair){
		return _putT(pair);
	}

	bool put(Pair &&pair){
		return _putT(std::move(pair));
	}

public:
	MultiTableIterator<LIST *> begin() const{
		return MultiTableIterator<LIST *>(_container);
	}

	MultiTableIterator<LIST *> end() const{
		return MultiTableIterator<LIST *>(_container, true);
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
	template <class UPAIR>
	bool _putT(UPAIR &&data);

private:
	LIST	_container[CAPACITY];
};

// ===================================

#include "hashlist_implementation.h"

#endif

