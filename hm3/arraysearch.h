#ifndef _IARRAY_H
#define _IARRAY_H

#include "stringref.h"
#include "checkedvalue.h"

namespace hm3{

namespace arraysearch{
	struct Linear{
		template <class ARRAY>
		CheckedValue<typename ARRAY::count_type, bool, true>	operator()(const ARRAY &array, const StringRef &key) const;
	};

	struct Binary{
		template <class ARRAY>
		CheckedValue<typename ARRAY::count_type, bool, true>	operator()(const ARRAY &array, const StringRef &key) const;
	};
}

} // namespace

// ==============================

#include "arraysearch_impl.h"

#endif

