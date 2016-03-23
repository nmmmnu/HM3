#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "ilist.h"

#include "multitableiterator.h"


namespace hm3{


template <class CONTAINER>
class HashList : public IMutableList<HashList<CONTAINER> >{
public:
	using List		= typename CONTAINER::value_type;
	using count_type	= typename List::count_type;
	using Iterator		= MultiTableIterator::Collection<CONTAINER>;

public:
	constexpr
	static size_t DEFAULT_SIZE = 64;

public:
	explicit
	HashList(size_t const size = 0) :
				_container(size ? size : DEFAULT_SIZE){}

	HashList(CONTAINER &&container) :
				_container(std::move(container)){}

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

	typename CONTAINER::size_type _getBucketForKey(const StringRef &key) const{
		/*
		Instead of modulo, this can be done with bit AND.
		Doing so, really increase the speed a bit,
		but code for file based operations get more complicated,
		and overal result will be low quality code.
		*/

		return _calcHash(key.data()) % _container.size();
	}

private:
	CONTAINER	_container;
};


} // namespace


#include "hashlist_implementation.h"

#endif

