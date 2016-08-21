#include "levelorderlookup.h"

#include <cstdio>

int main(){
	using namespace	hm3::btreeindex;

	LevelOrderLookup<7> lookup;

	for(branch_type i = 0; i < lookup.size(); ++i)
		printf("%4hu | %4hu\n", i, lookup[i] ); //, lookup.bw(i) );

}

