
template <class LIST>
static void op_list(const LIST &list, const StringRef &key, size_t const count){
	const auto bit = key == '-' ? list.begin() : list.lowerBound(key);
	const auto eit = list.end();

	size_t c = 0;
	for(auto it = bit; it != eit; ++it){
		it->print();

		if (++c >= count)
			break;
	}
}

template <class LIST>
static int op_list(const LIST &list, size_t const keyCount, char **keys, size_t const count = 10){
	for(size_t i = 0; i < keyCount; ++i)
		op_list(list, keys[i], count);

	return 0;
}

// =====================================

template <class LIST>
static int op_filesearch(const LIST &list, size_t const keyCount, char **keys){
	for(size_t i = 0; i < keyCount; ++i){
		const auto key = keys[i];

		const auto &pair = list.get(key);

		if ( pair )
			pair.print();
	}

	return 0;
}

