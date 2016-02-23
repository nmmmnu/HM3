#include "disktable.h"

template <class LSMC>
Pair LSMTable<LSMC>::get(const StringRef &key) const{
	for(const auto &table : _lsmc.get() ){
		if (Pair pair = table.get(key))
			return pair;
	}

	return nullptr;
}

template <class LSMC>
size_t LSMTable<LSMC>::getSize() const{
	count_type result = 0;

	for(const auto &table : _lsmc.get() )
		result += table.getSize();

	return result;
}

// ===================================

template <class LSMC>
auto LSMTable<LSMC>::getCountEstimated() const -> count_type{
	count_type result = 0;

	for(const auto &table : _lsmc.get() )
		result += table.getCount();

	return result;
}

template <class LSMC>
auto LSMTable<LSMC>::getCountReal() const -> count_type{
	count_type result = 0;

	auto endIt = end();
	for(auto it = begin(); it != endIt; ++it)
		++result;

	return result;
}


