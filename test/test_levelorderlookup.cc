#include "levelorderlookup.h"

#include <cstdio>

int main(){
	constexpr LevelOrderLookup<unsigned int, 7> ll;

	for(size_t i = 0; i < ll.size; ++i)
		printf("%4hu | %4hu\n", i, ll.value[i] );

}

