#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <vector>

template <class LIST>
class HashList : public IList<HashList<LIST> >{
public:
	using container_type = std::vector<LIST>;

	using count_type = typename HashList::count_type;

public:
	constexpr
	static size_t DEFAULT_SIZE = 1024;

public:
	explicit
	HashList(size_t const size = 0) :
				_container(size ? size : DEFAULT_SIZE){}

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const estimated = false) const noexcept;
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

	count_type _getBucketForKey(const StringRef &key) const{
		/*
		Instead of modulo, this can be done with bit AND.
		Doing so, really increase the speed a bit,
		but code for file based operations get more complicated,
		and overal result will be low quality code.
		*/

		return _calcHash(key.data()) % _container.size();
	}

private:
	container_type	_container;
};

// ===================================

#include "hashlist_implementation.h"

#endif

