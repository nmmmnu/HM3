#include "disktable.h"

template <class LSMCONTAINER>
Pair LSMTable<LSMCONTAINER>::get(const StringRef &key) const{
	for(const auto &table : *_container ){
		if (Pair pair = table.get(key))
			return pair;
	}

	return nullptr;
}

template <class LSMCONTAINER>
size_t LSMTable<LSMCONTAINER>::getSize() const{
	count_type result = 0;

	for(const auto &table : *_container )
		result += table.getSize();

	return result;
}

// ===================================

template <class LSMCONTAINER>
auto LSMTable<LSMCONTAINER>::getCountEstimated() const -> count_type{
	count_type result = 0;

	for(const auto &table : *_container )
		result += table.getCount();

	return result;
}

template <class LSMCONTAINER>
auto LSMTable<LSMCONTAINER>::getCountReal() const -> count_type{
	count_type result = 0;

	auto endIt = end();
	for(auto it = begin(); it != endIt; ++it)
		++result;

	return result;
}

