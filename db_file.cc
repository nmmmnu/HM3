#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <memory>
#include <fstream>

#include "diskfile.h"

#include "disktable.h"



static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s r [file.dat] [key] - load file.dat, then search for the key\n",			cmd);
	printf("\t%s l [file.dat] [key] - load file.dat, then list using iterator\n",			cmd);
	printf("\t%s L [file.dat] [key] - load file.dat, then list using iterator with start\n",	cmd);

	printf("\t\tFiles must be written without extention\n");
	printf("\t\tExample 'file.bin' instead of 'file.bin.meta'\n");

	printf("\n");
}



template <class LIST>
static void listSearch(LIST &list, const StringRef &key){
	const Pair pair = list.get(key);

	if (! pair){
		printf("Key '%s' not found...\n", key.data());
		return;
	}

	pair.print();
}



static int op_list(const StringRef &filename, const StringRef &key = StringRef(), size_t count = 100){
	DiskTable list;
	list.open(filename);
	list.print();

	for(const Pair &p : list ){
		p.print();

		if (--count == 0)
			break;
	}

	return 0;
}



static int op_filesearch(const StringRef &filename, const StringRef &key){
	DiskTable list;
	list.open(filename);
	list.print();

	listSearch(list, key);

	return 0;
}



int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 1;
	}

	const auto op		= argv[1];
	const auto filename	= argv[2];
	const auto key		= argv[3];

	switch(op[0]){
	case 'r':	return op_filesearch(
				filename,
				key
			);

	case 'L':	return op_list(
				filename,
				key
			);

	case 'l':	return op_list(
				filename
			);
	}
}

