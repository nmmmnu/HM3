#include "diskbtree/diskbtree.h"

#include "disktable.h"

#include <unistd.h>	// access

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("%s [file.db] btree_file", name);
	printf("\t\tPath names must be written without extention\n");
	printf("\t\tExample 'file.db'\n");
	printf("\t\tDo not forget that usually btree_file must have same name as [file.db]\n");
}

inline bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

int main(int argc, char **argv){
	if (argc <= 1 + 1){
		printUsage(argv[0]);
		return 1;
	}

	const char *input	= argv[1];
	const char *output	= argv[2];

#if 0
	if (fileExists(output)){
		printf("File %s exists. Please remove it and try again.\n", output);
		return 2;
	}
#endif

	hm3::DiskTable list;
	list.open(input);

	list.printHeader();

	hm3::diskbtree::DiskBTree btree(output);

	btree.createFromList(list);
}

