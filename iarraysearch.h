#ifndef _IARRAY_H
#define _IARRAY_H

#include "ilist.h"

#include <tuple>

namespace IArraySearch{
	typedef IListDefs::count_type count_type;

	struct Linear{
		template <class ARRAY>
		static std::tuple<int, count_type> processing(const ARRAY &array, const StringRef &key);
	};

	struct Binary{
		template <class ARRAY>
		static std::tuple<int, count_type> processing(const ARRAY &array, const StringRef &key);
	};
};

// ==============================

#include "iarraysearch_implementation.h"

#endif

