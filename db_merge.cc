#include "lsmtable.h"
#include "argtableloader.h"

#include "dualtable.h"

#include <unistd.h>	// access

static void printUsage(const char *name){
	const char *txt =
		"\t%s %c output_file [file1] [file2] [fileN...]"
		"- merge files, %-6s non valid, %-6s tombstones\n";

	printf("Usage:\n");
	printf(txt, 	name, '-', "keep",   "keep");
	printf(txt, 	name, 'v', "remove", "keep");
	printf(txt, 	name, 't', "remove", "remove");
	printf("\t\tFiles must be written with the extention.\n");
	printf("\t\tExample 'file.db'\n");
	printf("\t\tDo not forget you usually need two output files\n");
}

inline bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

template <class TABLE>
int merge(const TABLE &table, const char *output, bool const keepInvalid, bool const keepTombstones){
	DiskFile df = DiskFile(output);

	df.createFromList(table, keepInvalid, keepTombstones);

	return 0;
}

int main(int argc, char **argv){
	if (argc <= 1 + 1 + 1){
		printUsage(argv[0]);
		return 1;
	}

	bool keepInvalid	= true;
	bool keepTombstones	= true;

	switch(argv[1][0]){
	case 'v':
		keepInvalid	= false;
		keepTombstones	= true;
		break;

	case 't':
		keepInvalid	= false;
		keepTombstones	= false;
		break;
	}

	const char *output	= argv[2];

	const char **path	= (const char **) &argv[3];
	const int  pathc	= argc - 3;

	if (fileExists(output)){
		printf("File %s exists. Please remove it and try again.\n", output);
		return 2;
	}

	if (pathc == 1){
		const char *filename = path[0];

		DiskTable table;
		table.open(filename);

		printf("Merging (cleanup) single file...\n");
		printf("\t%s\n", filename);

		return merge(table, output, keepInvalid, keepTombstones);

	}else if (pathc == 2){
		const char *filename1 = path[0];
		const char *filename2 = path[1];

		DiskTable table1;
		table1.open(filename1);

		DiskTable table2;
		table2.open(filename2);

		DualTable<DiskTable, DiskTable> table(table1, table2);

		printf("Merging two files...\n");
		printf("\t%s\n", filename1);
		printf("\t%s\n", filename2);

		return merge(table, output, keepInvalid, keepTombstones);

	}else{
		ArgTableLoader al { pathc, path };

		LSMTable<ArgTableLoader::container_type> table( *al );

		printf("Merging multiple files...\n");

		return merge(table, output, keepInvalid, keepTombstones);
	}
}

