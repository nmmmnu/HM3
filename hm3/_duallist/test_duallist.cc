#include "duallist.h"

#include "skiplist.h"
#include "disktable.h"

constexpr const char *FILENAME	= "TEST_DATA.BIN/data.001.bin";
//constexpr const char *KEY	= "Sofia";

int main(){
	using MemList = hm3::SkipList;
	using hm3::DiskTable;
	using MyDualList = hm3::DualList<MemList,DiskTable> ;

	MemList memlist{};
	DiskTable disktable{};
	disktable.open(FILENAME);

	MyDualList dl(memlist, disktable);

	dl.emplace("Plovdiv", "new");

	dl.print();
}

