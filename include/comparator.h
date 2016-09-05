#ifndef COMPARATOR_H_
#define COMPARATOR_H_

#include "sgn.h"

//#include <cstdio>

namespace comparator{

template <class T>
int comp(const T &a, const T &b){
	if (a < b)
		return -1;

	if (a > b)
		return +1;

	return 0;
}

inline int comp(int const a, int const b){
//	printf("i");
	return a - b;
}

inline int comp(long int const a, long int const b){
//	printf("l");
	return sgn(a - b);
}

} // namespace

#endif

