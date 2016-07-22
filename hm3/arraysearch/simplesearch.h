#ifndef _SIMPLE_SEARCH_ARRAY_SEARCH_H
#define _SIMPLE_SEARCH_ARRAY_SEARCH_H

#include "base.h"

#include "stringref.h"

namespace hm3{
namespace arraysearch{


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

