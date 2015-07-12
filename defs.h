#ifndef _DEFS_H
#define _DEFS_H

#include <stdlib.h>	// size_t
#include <stdio.h>	// printf

#define xmalloc(s)	malloc(s)
#define xfree(a)	free(a)

static inline void my_error(const char *err, const char *file, unsigned int line){
	fprintf(stderr, "%s: key is NULL on %s:%u\n", err, file, line);
	exit(100);
}

static inline int SGN(int a){
	if (a == 0)
		return 0;

	return a > 0 ? 1 : -1;
}

#endif

