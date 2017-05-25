#ifndef _HASH_INDEX_BUILDER_H
#define _HASH_INDEX_BUILDER_H

#include "myhash.h"

#include "stringref.h"

#include "endian.h"

namespace hm3{
namespace hashindex{

class HashIndexBuilder{
private:
	constexpr static uint64_t BUCKET_EMPTY		= (uint64_t) -1;
	constexpr static uint64_t BUCKET_COLLISION	= (uint64_t) -2;

	constexpr static size_t DEFAULT_GROW_FACTOR	= 8;

	using MyHash = myhash::DJB2A<uint64_t>;

public:
	HashIndexBuilder(size_t const growFactor = DEFAULT_GROW_FACTOR) :
					growFactor_(growFactor){}

	template <class LIST>
	bool createFromList(const StringRef &filename, const LIST &list, bool stats = true) const;

private:
	static void printStats_(size_t const size, size_t const collisions);

	template <class LIST>
	size_t createFromList_(uint64_t *hashtable, size_t hashtableSize, const LIST &list) const;

	uint64_t bucketForKey_(const StringRef &key, size_t const hashtableSize) const;

private:
	size_t	growFactor_;
	MyHash	hash_;
};


} // namespace btreeindex
} // namespace


#include "hashindexbuilder_impl.h"

#endif

