#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <memory>
#include <fstream>

#include "disktable.h"
#include "diskfile.h"

#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#include "disktable.h"

#define PROCESS_STEP	1000 * 10

static void listLoad(IList &list, const StringRef &filename, bool tombstones = true);
static void listSearch(ITable &list, const StringRef &key);

static void printUsage(const char *cmd);

static std::unique_ptr<IList> factoryVector(char search){
	auto vlist = std::make_unique<VectorList>();
	vlist->setLookupMethod(search);
	return vlist;
}

static std::unique_ptr<IList> factory(char what){
	switch(what){
	case 'v':
		return factoryVector(VectorList::LINEAR_SEARCH);

	case 'V':
		return factoryVector(VectorList::BINARY_SEARCH);

	case 'l':
		return std::make_unique<LinkList>();

	default:
	case 's':
		return std::make_unique<SkipList>();
	}
}

static int op_search(IList &list, const StringRef &filename, const StringRef &key){
	printf("Load start...\n");
	listLoad(list, filename);
	printf("Load done...\n");
	getchar();

	printf("Search start...\n");
	listSearch(list, key);
	printf("Search done...\n");
	getchar();

	return 0;
}

static int op_write(IList &list, const StringRef &filename, const StringRef &filename2){
	printf("Load start...\n");
	listLoad(list, filename);
	printf("Load done...\n");
	getchar();

	printf("Write start...\n");
	DiskFile::create(filename2, list);
	printf("Write done...\n");
	getchar();

	return 0;
}

static int op_filesearch(const StringRef &filename, const StringRef &key){
	DiskTable list;

	list.open(filename);
	listSearch(list, key);

	return 0;
}

static int op_list(const StringRef &filename, const StringRef &key = StringRef(), size_t count = 10){
	DiskTable list;
	list.open(filename);

	auto it = list.getIterator();

	for(Pair pair = it->first(key); pair; pair = it->next()){
		pair.print();

		if (--count == 0)
			break;
	}

	return 0;
}

int main(int argc, char **argv){
	if (argc <= 4){
		printUsage(argv[0]);
		return 1;
	}

	const auto op		= argv[1];
	const auto what		= argv[2];
	const auto filename	= argv[3];
	const auto key		= argv[4];
	const auto filename2	= argv[4];

//	Pair::setChecksumUsage(false);

	switch(op[0]){
	case 's':	return op_search(
				*factory(what[0]),
				filename,
				key
			);

	case 'w':	return op_write(
				*factory(what[0]),
				filename,
				filename2
			);

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


	return 0;
}

static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s s [class] [file.txt] [key]      - load file.txt, then search for the key\n",		cmd);
	printf("\t%s w [class] [file.txt] [file.dat] - load file.txt, then create file.dat\n",			cmd);
	printf("\t%s r -       [file.dat] [key]      - load file.dat, then search for the key\n",		cmd);
	printf("\t%s l -       [file.dat] [key]      - load file.dat, then list using iterator\n",		cmd);
	printf("\t%s L -       [file.dat] [key]      - load file.dat, then list using iterator with start\n",	cmd);
	printf("Classes are:\n");
	printf("\t%c - %s\n", 'V', "VectorList bin search");
	printf("\t%c - %s\n", 'v', "VectorList linear search");
	printf("\t%c - %s\n", 'l', "LinkList");
	printf("\t%c - %s\n", 's', "SkipList");
	printf("\n");
}

static void listLoad(IList &list, const StringRef &filename, bool const tombstones){ 
	static const char *trim_ch = " \t\n";

	const std::string empty;

	std::ifstream f;
	f.open(filename);

	unsigned int i = 0;
	
	for(std::string line; getline(f, line);){
		// trim
		line.erase(line.find_last_not_of(trim_ch) + 1);

		const std::string &key = line;
		const std::string &val = tombstones ? empty : key;

		list.put( Pair{ key, val } );

		++i;

		if (i % ( PROCESS_STEP ) == 0){
			printf("Processed %10u records, %10zu bytes...\n", i, list.getSize() );
		}
	}
}

static void listSearch(ITable &list, const StringRef &key){
	const Pair pair = list.get(key);

	if (! pair){
		printf("Key '%s' not found...\n", key.data());
		return;
	}

	pair.print();
}
