#include "disktable.h"

template <class CONTAINER>
Pair LSMTable<CONTAINER>::get(const StringRef &key) const{
	for(const DiskTable &dt : _container){
		if (Pair pair = dt.get(key))
			return pair;
	}

	return nullptr;
}

template <class CONTAINER>
size_t LSMTable<CONTAINER>::getSize() const{
	size_t result = 0;

	for(const auto &table : _container)
		result += table.getSize();

	return result;
}

// ===================================

template <class CONTAINER>
size_t LSMTable<CONTAINER>::getCountEstimated() const{
	size_t result = 0;

	for(const auto &table : _container)
		result += table.getCount();

	return result;
}

template <class CONTAINER>
size_t LSMTable<CONTAINER>::getCountReal() const{
	size_t result = 0;

	auto endIt = end();
	for(auto it = begin(); it != endIt; ++it)
		++result;

	return result;
}
