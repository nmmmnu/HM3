#ifndef _BASE_ARRAY_SEARCH_H
#define _BASE_ARRAY_SEARCH_H

#include "stringref.h"
#include "checkedvalue.h"

namespace hm3{
namespace arraysearch{


template <class ARRAY>
using result_type = CheckedValue<typename ARRAY::count_type, bool, true>;


template <class LOOKUP>
struct SimpleSearch{
	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &self, const StringRef &key) const{
		auto const count = self.getCount();

		if (count == 0)
			return {false, 0u};

		return lookup_(self, key, 0, count);
	}

private:
	LOOKUP	lookup_;
};


} // namespace arraysearch
} // namespace

#endif

