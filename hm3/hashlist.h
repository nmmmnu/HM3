#ifndef _HASH_LIST_H
#define _HASH_LIST_H

#include "djbhash.h"

#include "ilist.h"
#include "multitableiterator/collectioniterator.h"


namespace hm3{


template <class CONTAINER>
class HashList : public IMutableList<HashList<CONTAINER> >{
public:
	using List		= typename CONTAINER::value_type;
	using size_type		= typename List::size_type;
	using Iterator		= multitableiterator::CollectionIterator<CONTAINER>;

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

	size_type getCount(bool const estimated = false) const noexcept;
	size_t getBytes() const noexcept;

private:
	friend class IMutableList<HashList<CONTAINER> >;

	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	Iterator lowerBound(const StringRef &key) const{
		return Iterator(container_, key);
	}

	Iterator begin() const{
		return Iterator(container_);
	}

	Iterator end() const{
		return Iterator(container_, true);
	}

private:
	static unsigned long calcHash_(const char *str){
		return DJBHash(str);
	}

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

