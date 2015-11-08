#ifndef _IARRAY_H
#define _IARRAY_H

#include "ilist.h"

#include <tuple>

namespace IArraySearch{
	typedef ListDefs::count_type count_type;
	typedef std::tuple<int, ListDefs::count_type> result_type;

	struct Linear{
		template <class ARRAY>
		static result_type processing(const ARRAY &array, const StringRef &key);
	};

	struct Binary{
		template <class ARRAY>
		static result_type processing(const ARRAY &array, const StringRef &key);
	};
};

// ==============================

#include "iarraysearch_implementation.h"

#endif

