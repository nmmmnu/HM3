//#ifndef _DB_FILE_IMPLEMENTATION_H
//#define _DB_FILE_IMPLEMENTATION_H

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

//#endif
