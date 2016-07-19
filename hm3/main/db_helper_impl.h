using Pair = hm3::Pair;

template <class LIST>
static int op_list(const LIST &list, const StringRef &key = StringRef(), size_t const count = 100){
	list.print(count);

	return 0;
}

template <class LIST>
static int op_filesearch(const LIST &list, const StringRef &key){
	const Pair pair = list.get(key);

	if ( ! pair ){
		printf("Key '%s' not found...\n", key.data());
		return 1;
	}

	pair.print();

	return 0;
}

static std::string &trim(std::string &line) __attribute__((unused));

static std::string &trim(std::string &line){
	constexpr const char *trim_ch = " \t\r\n";

	line.erase(line.find_last_not_of(trim_ch) + 1);

	return line;
}

