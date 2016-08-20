#ifndef BTREE_INDEX_NODE_H
#define BTREE_INDEX_NODE_H

#include <cstdint>

namespace hm3{
namespace btreeindex{


using level_type	= uint8_t;
using branch_type	= uint16_t;
using offset_type	= uint64_t;


// 512 would will fit in 4K page, but keys are unknown size.
// because of in-node binary search, best to be power of 2 - 1
constexpr level_type	NODE_LEVELS	= 3;
constexpr branch_type	BRANCHES	= 1 << NODE_LEVELS;
constexpr branch_type	VALUES		= BRANCHES - 1;

constexpr branch_type	LEAFMARK	= VALUES + 1;


struct NodeData{
	uint16_t	keysize;		// 2
	uint64_t	dataid;			// 8
} __attribute__((__packed__));


struct Node{
	constexpr static uint64_t NIL = (uint64_t) -1;

	uint16_t	size;			// 2

	uint8_t		pad_01_[6]	= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
						// 6 padding

	uint64_t	values[VALUES];		// 8 * VALUES
} __attribute__((__packed__));


} // namespace diskbtree
} // namespace


#endif

