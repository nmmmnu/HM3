#include <stdio.h>	// printf
#include <ctype.h>	// isspace

#include "lsmtable.h"
#include "filecontainerdirectory.h"

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("\t%s s [directory] [key] - open directory then search for the key\n", 		name);
	printf("\t%s l [directory] -     - open directory then List dir using iterator\n",	name);
	printf("\t%s L [directory] [key] - open directory then List dir using iterator\n",	name);
	printf("\t\tDo not forget about quotes around the directory\n");
}

template <class LIST>
void find(const LIST &list, const StringRef &key){
	const Pair &pair = list.get(key);

	if (! pair){
		printf("Key '%s' not found...\n", key.data());
		return;
	}

	pair.print();
}

template <class LIST>
void listing(const LIST &list, const StringRef &key = StringRef(), size_t count = 100){
	auto it_end = list.end();

	for(auto it = list.begin(); it != it_end; ++it){
		const Pair &pair = *it;

		pair.print();

		if (--count == 0)
			break;
	}
}

int main(int argc, char **argv){
	if (argc <= 3){
		printUsage(argv[0]);
		return 1;
	}

	const auto op	= argv[1];
	const auto path	= argv[2];
	const auto key	= argv[3];

	typedef std::vector<DiskTable> MyDTVector;

	MyDTVector container;

	loadFileContainerFromDirectory(container, path);

	LSMTable<MyDTVector> mt(container);

	switch(op[0]){
		case 's':
			find(mt, key);
			break;

		case 'l':
			listing(mt);
			break;

		case 'L':
			listing(mt, key);
			break;

		default:
			printUsage(argv[0]);
			return 1;
	}
}
