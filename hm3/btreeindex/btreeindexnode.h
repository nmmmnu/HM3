#ifndef BTREE_INDEX_NODE_H
#define BTREE_INDEX_NODE_H

#include <cstdint>

namespace hm3{
namespace btreeindex{


using branch_type	= uint16_t;
using offset_type	= uint64_t;


// 255 would fit in 4K page, but keys are unknown size.
constexpr branch_type	VALUES		= 200;
constexpr branch_type	BRANCHES	= VALUES + 1;


struct NodeValue{
	uint64_t	key;			// 8
	uint64_t	data;			// 8
	uint16_t	keysize;		// 2
} __attribute__((__packed__));


struct Node{
	uint16_t	size;			// 2
	uint8_t		leaf;			// 1
	uint8_t		al_01_	= 0xAA;		// 1
	NodeValue	values[VALUES];		// 16 * VALUES
} __attribute__((__packed__));


} // namespace diskbtree
} // namespace


#endif

