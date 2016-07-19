#include "stringtokenizer.h"

#include <assert.h>

static int test_stringtokenizer(){
	const char *s1 = "hello world";

	StringTokenizer st1{ s1 };

	assert(st1.getNext() == "hello");
	assert(st1.getNext() == "world");
	assert(st1.getNext().empty());
	assert(st1.getNext().empty());
	assert(st1.getNext().empty());

	const auto v1 = st1.getAll();

	assert(v1.size() == 2);
	assert(v1[0] == "hello");
	assert(v1[1] == "world");

	const char *s2 = " hello world ";

	StringTokenizer st2{ s2 };

	assert(st2.getNext().empty());
	assert(st2.getNext() == "hello");
	assert(st2.getNext() == "world");
	assert(st2.getNext().empty());
	assert(st2.getNext().empty());
	assert(st2.getNext().empty());

	const auto v2 = st2.getAll();

	assert(v2.size() == 3);
	assert(v2[0] == "");
	assert(v2[1] == "hello");
	assert(v2[2] == "world");

	return 0;
}

int main(){
	return test_stringtokenizer();
}

