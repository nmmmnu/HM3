#include "djbhash.h"

unsigned long DJBHash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	for(const char *c = str; *c; ++c)
		hash = ((hash << 5) + hash) + (unsigned long) *c;

	return hash;
}

