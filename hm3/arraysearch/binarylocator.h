#ifndef _BinaryLocator_ARRAY_SEARCH_H
#define _BinaryLocator_ARRAY_SEARCH_H

#include "simplesearch.h"

namespace hm3{
namespace arraysearch{


struct BinaryLocator{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;

};


} // namespace arraysearch
} // namespace

// ==============================

#include "binarylocator_impl.h"

#endif

