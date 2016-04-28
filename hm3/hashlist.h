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
				container_(size ? size : DEFAULT_SIZE){}

	HashList(CONTAINER &&container) :
				container_(std::move(container)){}

public:
	bool removeAll();

	const Pair &get(const StringRef &key) const;
	bool remove(const StringRef &key);

	count_type getCount(bool const estimated = false) const noexcept;
	size_t getSize() const noexcept;

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	Iterator begin() const{
		return Iterator(container_);
	}

	Iterator end() const{
		return Iterator(container_, true);
	}

private:
	static unsigned long calcHash_(const char *str);

	typename CONTAINER::size_type getBucketForKey_(const StringRef &key) const{
		/*
		Instead of modulo, this can be done with bit AND.
		Doing so, really increase the speed a bit,
		but code for file based operations get more complicated,
		and overal result will be low quality code.
		*/

		return calcHash_(key.data()) % container_.size();
	}

private:
	CONTAINER	container_;
};


} // namespace


#include "hashlist_impl.h"

#endif
