#ifndef _LINEAR_ARRAY_SEARCH_H
#define _LINEAR_ARRAY_SEARCH_H

#include "arraysearch/base.h"

namespace hm3{
namespace arraysearch{


struct Linear{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &array, const StringRef &key,
							typename ARRAY::count_type left,
							typename ARRAY::count_type right) const;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "linear_impl.h"

#endif

