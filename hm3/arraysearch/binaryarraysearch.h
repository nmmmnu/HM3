#ifndef _BINARY_ARRAY_SEARCH_H
#define _BINARY_ARRAY_SEARCH_H

#include "stringref.h"
#include "checkedvalue.h"

namespace hm3{
namespace arraysearch{


struct Binary{
	template <class ARRAY>
	CheckedValue<typename ARRAY::count_type, bool, true>	operator()(const ARRAY &array, const StringRef &key) const;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "binaryarraysearch_impl.h"

#endif

