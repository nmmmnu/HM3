#ifndef _BASE_ARRAY_SEARCH_H
#define _BASE_ARRAY_SEARCH_H

#include "checkedvalue.h"

namespace hm3{
namespace arraysearch{


template <class ARRAY>
using result_type = CheckedValue<typename ARRAY::size_type, bool, true>;


} // namespace arraysearch
} // namespace

#endif

