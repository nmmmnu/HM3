inline int binarySearchComparator(int const &a, int const &b);

#include "binarysearch.h"

#include <cstdio>

template<class T, size_t SIZE>
class VVector{
public:
	size_t size() const{
		return SIZE;
	}

	T operator [](size_t const index) const{
		return T(index);
	}
};


inline int binarySearchComparator(int const &a, int const &b){
	printf(".");
	return a - b;
}

constexpr unsigned short MAX = 10000;

template<class T>
void test(const char *name, const T key){
	VVector<T, MAX> v;

	size_t result;

	bool const found = binarySearch(v, v.size(), key, BinarySearchCompStdandard{}, result);

	bool const ok = found && v[result] == key;

	printf("%10s %s\n", name, ok ? "OK" : "Error");
}

template<class T>
void test(const char *name){
	test(name, (T) 0);
	test(name, (T) 100);
	test(name, (T) MAX - 1);
}

int main(){
	test<short>("short");
	test<int>("int");
	test<long>("long");

	test<unsigned short>("u short");
	test<unsigned int>("u int");
	test<unsigned long>("u long");

	test<size_t>("size_t");
};

