using Pair = hm3::Pair;

template <class LIST>
static void op_list(const LIST &list, const StringRef &key, size_t const count){
	const auto bit = key == '-' ? list.begin() : list.getIterator(key);
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

		const Pair pair = list.get(key);

		if ( pair )
			pair.print();
	}

	return 0;
}

// =====================================

static std::string &trim(std::string &line) __attribute__((unused));

static std::string &trim(std::string &line){
	constexpr const char *trim_ch = " \t\r\n";

	line.erase(line.find_last_not_of(trim_ch) + 1);

	return line;
}

