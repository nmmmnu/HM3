#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <vector>

template <class LIST>
class HashList : public IMutableList<HashList<LIST> >{
public:
	using container_type	= std::vector<LIST>;
	using count_type	= typename HashList::count_type;
	using Iterator		= MultiTableIterator::Collection<container_type>;

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
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data);

public:
	Iterator begin() const{
		return Iterator(_container);
	}

	Iterator end() const{
		return Iterator(_container, true);
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

