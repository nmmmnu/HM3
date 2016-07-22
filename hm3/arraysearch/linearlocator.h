#ifndef _LinearLocator_ARRAY_SEARCH_H
#define _LinearLocator_ARRAY_SEARCH_H

#include "base.h"

#include "stringref.h"

namespace hm3{
namespace arraysearch{


struct LinearLocator{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "linearlocator_impl.h"

#endif

