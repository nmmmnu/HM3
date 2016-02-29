
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

static std::pair<StringRef, StringRef> explode(const std::string &line, const char *delimiters = ":") __attribute__((unused));

static std::pair<StringRef, StringRef> explode(const std::string &line, const char *delimiters){
	constexpr const char *trim_ch		= "\t\r\n ";

	size_t const tr = line.find_last_not_of(trim_ch) + 1;
	size_t const ix = line.find_first_of(delimiters);

	if (ix >=  tr){ // std::string::npos
		return std::make_pair(
			StringRef( line.c_str() ),
			StringRef()
		);
	}

	return std::make_pair(
			StringRef( &line.c_str()[0     ], ix - 1),
			StringRef( &line.c_str()[ix + 1]        )
	);
}
