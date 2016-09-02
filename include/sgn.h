#ifndef SGN_H_
#define SGN_H_

template<typename T>
int sgn(const T a) noexcept{
	return (T(0) < a) - (a < T(0));
}

#endif

