#ifndef _DISK_BTREE_HEADER_H
#define _DISK_BTREE_HEADER_H

#include <cstdint>

namespace hm3{
namespace diskbtree{

class DiskBTreeNode{
public:
	using branch_type	= uint16_t;
	using offset_type	= uint64_t;

	// 255 would fit in 4K page, but keys are unknown size.
	constexpr static branch_type	VALUES		= 200;
	constexpr static branch_type	BRANCHES	= VALUES + 1;

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

public:

};

} // namespace diskbtree
} // namespace


#endif

