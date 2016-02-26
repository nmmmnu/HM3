#include "duallist.h"

#include "skiplist.h"
#include "disktable.h"

#include <cstdio>

constexpr const char *FILENAME	= "TEST_DATA.BIN/data.001.db";
constexpr const char *KEY	= "Sofia";

constexpr size_t max_size	= 10 * 1024 * 1024;

int main(){
	DualList<SkipList,DiskTable> dl(SkipList{}, DiskTable{}, max_size);

	dl.getTable().open(FILENAME);

	dl.emplace("Plovdiv", "new");

	dl.print();

/*
	for(unsigned long i = 0; i < 1 * 1000 * 1000; ++i){
		std::string key = "Item #";
		key += std::to_string(i);

		Pair p = { key, key };

		if (i % 50000 == 0){
			printf("%8lu pairs processed, %8zu KB\n", i, dl.getMemList().getSize() / 1024 );
		}

		dl.put(std::move(p));
	}
*/
}

