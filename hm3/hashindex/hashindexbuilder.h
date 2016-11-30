#ifndef _HASH_INDEX_BUILDER_H
#define _HASH_INDEX_BUILDER_H

#include "stringref.h"

#include "endian.h"

namespace hm3{
namespace hashindex{

class HashIndexBuilder{
private:
	constexpr static uint64_t BUCKET_EMPTY		= (uint64_t) -1;
	constexpr static uint64_t BUCKET_COLLISION	= (uint64_t) -2;

	constexpr static size_t DEFAULT_GROW_FACTOR	= 8;

public:
	HashIndexBuilder(size_t const growFactor = DEFAULT_GROW_FACTOR) :
					growFactor_(growFactor){}

	template <class LIST>
	bool createFromList(const StringRef &filename, const LIST &list, bool stats = true);

private:
	static void printStats_(size_t const size, size_t const collisions);

	template <class LIST>
	size_t createFromList_(uint64_t *hashtable, size_t hashtableSize, const LIST &list);

private:
	size_t growFactor_;
};


} // namespace btreeindex
} // namespace


#include "hashindexbuilder_impl.h"

#endif

