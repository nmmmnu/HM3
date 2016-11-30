#include "btreeindex/btreeindexbuilder.h"

#include "disktable.h"

#include <unistd.h>	// access

constexpr bool CHECK_FILE_EXISTS = false;

static void printUsage(const char *name){
	printf("Usage:\n");
	printf("%s [file.db] [btree_file]\n", name);
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

	const char *input_file	= argv[1];
	const char *output_file	= argv[2];

	if (CHECK_FILE_EXISTS && fileExists(output_file)){
		printf("File %s exists. Please remove it and try again.\n", output_file);
		return 2;
	}

	using DiskTable = hm3::DiskTable;

	hm3::DiskTable list;
	list.open(input_file);

	using BTreeIndexBuilder = hm3::btreeindex::BTreeIndexBuilder<DiskTable>;

	BTreeIndexBuilder builder;

	bool const result = builder.createFromList(output_file, list);

	return result ? 0 : 100;
}

