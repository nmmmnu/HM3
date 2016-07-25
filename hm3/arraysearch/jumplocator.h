#ifndef JUMP_LOCATOR_ARRAY_SEARCH_H_
#define JUMP_LOCATOR_ARRAY_SEARCH_H_

#include "binarylocator.h"

namespace hm3{
namespace arraysearch{

/*
 * JumpLocator is very similar to PartitionSearch,
 * except it reli on the OS for caching the partitions.
 * Performance is very similar, but it have following advantages:
 *	- no need to store the keys into memory.
 *	- underline list does not need to be immutable.
 *	- gradual prefetch - it does not need to prefetch whole DiskTable.
 *
 * Check the stats at the end of the file.
 *
 */

class JumpLocator{
public:
	using my_size_type = size_t;

	static constexpr my_size_type	DEFAULT_MINIMAL_JUMP	= 1000 * 1000;
	static constexpr my_size_type	DEFAULT_JUMPS		= 500;

public:
	JumpLocator(my_size_type const jumps = DEFAULT_JUMPS, my_size_type const minimalJump = DEFAULT_MINIMAL_JUMP) :
							jumps_(jumps){}

public:
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &list, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;

private:
	template <class ARRAY>
	typename ARRAY::size_type calcJump_(const ARRAY &list) const;

private:
	my_size_type	jumps_;

	BinaryLocator	locator_;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "jumplocator_impl.h"

#endif

#if 0

LSM on 6 huge tables:
==============================
Class				Disk	Parts		VM Stats	Prefetch/Heat	Search
PartitionSearch<BinarySearch>	RAID5	500		7G/228G		1:12		0:02.5
SimpleSearch<JumpSearch>	RAID5	500		7G/228G		0:54		0:02.5
SimpleSearch<JumpSearch>	SSD	500		170M/228G	0:00.8		0:00.5
SimpleSearch<JumpSearch>	USB2	500		170M/228G	0:05		0:00.5

LSM on 230 tables ~8M keys each:
==============================
Class				Disk	Parts		VM Stats	Prefetch/Heat	Search
PartitionSearch<BinarySearch>	RAID5	100		242M/128G	3:45		0:14
PartitionSearch<BinarySearch>	RAID5	500		912M/128G	8:15		0:09
SimpleSearch<JumpSearch>	RAID5	500		6G/128G		1:00		0:20
SimpleSearch<JumpSearch>	SSD	500		230M/128G	0:01		0:01

==============================
Disks are:
	RAID5	Western Digital Red,
			4 x 4 TB + 1 x 6 TB,
			connection over SATA3
	SSD	Samsung 850 EVO, 250 GB,
			connection over SATA3
	USB2	Samsung 850 EVO, 250 GB,
			connection over USB2

#endif

