#ifndef JUMP_LOCATOR_ARRAY_SEARCH_H_
#define JUMP_LOCATOR_ARRAY_SEARCH_H_

#include "binarylocator.h"

namespace hm3{
namespace arraysearch{


class JumpLocator{
public:
	using my_size_type = size_t;

	static constexpr my_size_type	DEFAULT_JUMP = 1000 * 1000;

public:
	JumpLocator(my_size_type const jump = DEFAULT_JUMP) : jump_(jump){}

public:
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;

private:
	my_size_type	jump_;

	BinaryLocator	locator_;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "jumplocator_impl.h"

#endif

