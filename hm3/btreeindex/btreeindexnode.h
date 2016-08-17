#ifndef BTREE_INDEX_NODE_H
#define BTREE_INDEX_NODE_H

#include <cstdint>

namespace hm3{
namespace btreeindex{


using branch_type	= uint16_t;
using offset_type	= uint64_t;


// 512 would fit in 4K page, but keys are unknown size.
constexpr branch_type	VALUES		= 511;
constexpr branch_type	BRANCHES	= VALUES + 1;

constexpr branch_type	LEAFMARK	= VALUES + 1;


struct NodeData{
	uint16_t	keysize;		// 2
	uint64_t	dataid;			// 8
} __attribute__((__packed__));


struct Node{
	uint16_t	size;			// 2

	uint16_t	pad_01_	= 0xAAaa;	// 2 padding
	uint32_t	pad_02_	= 0xAAaaAAaa;	// 4 padding

	uint64_t	values[VALUES];		// 8 * VALUES
} __attribute__((__packed__));


} // namespace diskbtree
} // namespace


#endif

