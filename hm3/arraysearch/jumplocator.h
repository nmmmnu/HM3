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
 */

class JumpLocator{
public:
	using my_size_type = size_t;

	static constexpr my_size_type	MINIMAL_JUMP		= 1000 * 1000;
	static constexpr my_size_type	DEFAULT_JUMPS		= 500;

	/*
	LSM on 6 huge files:
	PartitionSearch<BinarySearch>
	500		7G/228G		1:12		0:025

	SimpleSearch<JumpSearch>
	500		7G/228G		0:54		0:025
	*/

public:
	JumpLocator(my_size_type const jumps = DEFAULT_JUMPS) : jumps_(jumps){}

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

