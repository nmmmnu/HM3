#ifndef _BinaryLocator_ARRAY_SEARCH_H
#define _BinaryLocator_ARRAY_SEARCH_H

#include "linearlocator.h"

namespace hm3{
namespace arraysearch{


template<general_size_type MINIMUM_DISTANCE>
struct HybridBinaryLocator{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;

private:
	LinearLocator		locator_;
};

using BinaryLocator = HybridBinaryLocator<8>;

} // namespace arraysearch
} // namespace

// ==============================

#include "binarylocator_impl.h"

#endif

