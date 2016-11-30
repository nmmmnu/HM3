#include "myhash.h"

unsigned long DJB2Hash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	for(const char *c = str; *c; ++c)
		hash = ((hash << 5) + hash) + (unsigned long) *c;

	return hash;
}

unsigned long DJB2AHash(const char *str){
	// mysterious DJB const
	unsigned long hash = 5381;

	for(const char *c = str; *c; ++c)
		hash = ((hash << 5) + hash) ^ (unsigned long) *c;

	return hash;
}

unsigned long SDBMHash(const char *str){
	unsigned long hash = 0;

//	int c;
//	while (c = *str++)
	for(const char *c = str; *c; ++c)
		hash = (hash << 6) + (hash << 16) - hash + (unsigned long) *c;

        return hash;
}

