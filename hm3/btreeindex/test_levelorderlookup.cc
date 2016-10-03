#include "levelorderlookup.h"

#include <cstdio>

int main(){
	using namespace hm3::btreeindex;

	constexpr LevelOrderLookup<7> ll;

	for(branch_type i = 0; i < ll.size; ++i)
		printf("%4hu | %4hu\n", i, ll.value[i] );

}

