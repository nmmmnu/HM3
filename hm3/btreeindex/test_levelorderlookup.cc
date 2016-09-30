#include "levelorderlookup.h"

#include <cstdio>

int main(){
	using namespace	hm3::btreeindex;

	LevelOrderLookup<7> lookupHolder;
	const auto &table = lookupHolder.get();

	for(branch_type i = 0; i < table.size(); ++i)
		printf("%4hu | %4hu\n", i, table[i] );

}

