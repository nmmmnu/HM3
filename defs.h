#ifndef _DEFS_H
#define _DEFS_H

static inline void *xmalloc(size_t s){
	return malloc(s);
}

static inline void xfree(void *a){
	free(a);
}

static inline void *xrealloc(void *a, size_t size){
	return realloc(a, size);
}

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

