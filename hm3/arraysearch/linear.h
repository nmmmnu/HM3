#ifndef _LINEAR_ARRAY_SEARCH_H
#define _LINEAR_ARRAY_SEARCH_H

#include "stringref.h"
#include "checkedvalue.h"

namespace hm3{
namespace arraysearch{


struct Linear{
	template <class ARRAY>
	CheckedValue<typename ARRAY::count_type, bool, true>	operator()(const ARRAY &array, const StringRef &key) const;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "linear_impl.h"

#endif

