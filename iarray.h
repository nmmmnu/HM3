#ifndef _IARRAY_H
#define _IARRAY_H

#include "ilist.h"

#include <tuple>

template <class T, class LOOKUP>
class IArray{
public:
	typedef IListDefs::count_type count_type;

public:
	Pair get(const StringRef &key) const{
		const auto l = lookup(key);
		return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
	}

	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

public:
	Pair getAt(count_type const index) const{
		return static_cast<const T*>(this)->getAt(index);
	}

	int  cmpAt(count_type const index, const StringRef &key) const{
		return static_cast<const T*>(this)->cmpAt(index, key);
	}

	count_type getCount() const{
		return static_cast<const T*>(this)->getCount();
	}
};

// ==============================

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

#include "iarray_implementation.h"

#endif

