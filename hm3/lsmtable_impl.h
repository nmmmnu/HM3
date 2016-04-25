#include "disktable.h"


namespace hm3{


template <class CONTAINER>
Pair LSMTable<CONTAINER>::get(const StringRef &key) const{
	// CONTAINER is responsible for ordering the tables,
	// in correct (probably reverse) order.
	for(const auto &table : container_ ){
		if (Pair pair = table.get(key))
			return pair;
	}

	return nullptr;
}

template <class CONTAINER>
size_t LSMTable<CONTAINER>::getSize() const{
	count_type result = 0;

	for(const auto &table : container_ )
		result += table.getSize();

	return result;
}

// ===================================

template <class CONTAINER>
auto LSMTable<CONTAINER>::getCountEstimated() const -> count_type{
	count_type result = 0;

	for(const auto &table : container_ )
		result += table.getCount();

	return result;
}

template <class CONTAINER>
auto LSMTable<CONTAINER>::getCountReal() const -> count_type{
	count_type result = 0;

	auto endIt = end();
	for(auto it = begin(); it != endIt; ++it)
		++result;

	return result;
}


} // namespace

