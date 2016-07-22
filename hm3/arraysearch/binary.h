#ifndef _BINARY_ARRAY_SEARCH_H
#define _BINARY_ARRAY_SEARCH_H

#include "arraysearch/base.h"

namespace hm3{
namespace arraysearch{


struct Binary{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::size_type left,
							typename ARRAY::size_type right) const;

};


} // namespace arraysearch
} // namespace

// ==============================

#include "binary_impl.h"

#endif

