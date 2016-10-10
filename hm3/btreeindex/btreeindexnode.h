#ifndef BTREE_INDEX_NODE_H
#define BTREE_INDEX_NODE_H

#include "btreeindextypes.h"

namespace hm3{
namespace btreeindex{

// 512 would will fit in 4K page, but keys are unknown size.
// because of in-node binary search, best to be power of 2 - 1
constexpr level_type	NODE_LEVELS	= 7;
constexpr branch_type	BRANCHES	= 1 << NODE_LEVELS;
constexpr branch_type	VALUES		= BRANCHES - 1;


struct NodeData{
	uint16_t	keysize;		// 2
	uint64_t	dataid;			// 8
} __attribute__((__packed__));


struct Node{
	constexpr static uint64_t NIL		= (uint64_t) -1;

	uint64_t	values[VALUES];		// 8 * VALUES
} __attribute__((__packed__));


} // namespace diskbtree
} // namespace


#endif

