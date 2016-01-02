#include <cstdio>	// printf
#include <ctype.h>	// isspace

#include <memory>
#include <fstream>

#include "diskfile.h"

#include "stlvectorlist.h"
#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#include "hashlist.h"

#include "disktable.h"

#define PROCESS_STEP	1000 * 10




static void printUsage(const char *cmd){
	printf("Usage:\n");
	printf("\t%s s [class] [file.txt] [key]      - load file.txt, then search for the key\n",		cmd);
	printf("\t%s w [class] [file.txt] [file.dat] - load file.txt, then create file.dat\n",			cmd);
	printf("Classes are:\n");
	printf("\t%c - %s\n", 'T', "std::VectorList bin search");
	printf("\t%c - %s\n", 't', "std::VectorList linear search");
	printf("\t%c - %s\n", 'V', "VectorList bin search");
	printf("\t%c - %s\n", 'v', "VectorList linear search");
	printf("\t%c - %s\n", 'w', "HashList@VectorList bin search");
	printf("\t%c - %s\n", 'l', "LinkList");
	printf("\t%c - %s\n", 's', "SkipList");
	printf("\t%c - %s\n", 'z', "HashList@SkipList");
	printf("\n");
}



template <class LIST>
static void listLoad(LIST &list, const StringRef &filename, bool const tombstones = true){
	static const char *trim_ch = " \t\n";

	std::ifstream f;
	f.open(filename);

	unsigned int i = 0;

	for(std::string line; getline(f, line);){
		// trim
		line.erase(line.find_last_not_of(trim_ch) + 1);

		const StringRef key = line;
		const StringRef val = tombstones ? nullptr : key;

		if (! key.empty())
			list.put( Pair{ key, val } );

		++i;

		if (i % ( PROCESS_STEP ) == 0){
			printf("Processed %10u records, %10zu bytes...\n", i, list.getSize() );
		}
	}
}

template <class LIST>
static void listSearch(const LIST &list, const StringRef &key){
	const Pair pair = list.get(key);

	if (! pair){
		printf("Key '%s' not found...\n", key.data());
		return;
	}

	pair.print();
}





template <class LIST>
static int op_write(LIST &&list, const StringRef &filename, const std::string &filename2){
	printf("Load start...\n");
	listLoad(list, filename);
	printf("Load done...\n");
	getchar();

	printf("Write start...\n");
	DiskFile df = DiskFile(filename2);
	df.createFromList(list, true, true);
	printf("Write done...\n");
	getchar();

	return 0;
}

static int op_write_v(char const what, const StringRef &filename, const StringRef &filename2){
	switch(what){
	case 'v':
	case 'V':	return op_write(VectorList<>{},	filename, filename2);

	case 'l':	return op_write(LinkList{},	filename, filename2);

	default:
	case 's':	return op_write(SkipList{},	filename, filename2);
	}
}



template <class LIST>
static int op_search(LIST &&list, const StringRef &filename, const StringRef &key){
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

static int op_search_v(char const what, const StringRef &filename, const StringRef &key){
	constexpr auto buckets = 8192;

	switch(what){

	case 't':	return op_search(STLVectorList<IArraySearch::Linear>{},			filename, key);
	case 'T':	return op_search(STLVectorList<IArraySearch::Binary>{},			filename, key);

	case 'v':	return op_search(VectorList<IArraySearch::Linear>{},			filename, key);
	case 'V':	return op_search(VectorList<IArraySearch::Binary>{},			filename, key);
	case 'w':	return op_search(HashList<VectorList<IArraySearch::Binary> >{buckets},	filename, key);

	case 'l':	return op_search(LinkList{},						filename, key);

	default:
	case 's':	return op_search(SkipList{},						filename, key);
	case 'z':	return op_search(HashList<SkipList>{buckets},				filename, key);

	}
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

	switch(op[0]){
	case 's':	return op_search_v(
				what[0],
				filename,
				key
			);

	case 'w':	return op_write_v(
				what[0],
				filename,
				filename2
			);
	}
}

