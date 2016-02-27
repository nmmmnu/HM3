#ifndef _IARRAY_H
#define _IARRAY_H

#include "ilist.h"

#include <tuple>

namespace IArraySearch{

	struct Linear{
		template <class ARRAY>
		typename std::tuple<int, typename ARRAY::count_type>   operator()(const ARRAY &array, const StringRef &key) const;
	};

	struct Binary{
		template <class ARRAY>
		typename std::tuple<int, typename ARRAY::count_type>   operator()(const ARRAY &array, const StringRef &key) const;
	};
};

// ==============================

#include "iarraysearch_implementation.h"

#endif

