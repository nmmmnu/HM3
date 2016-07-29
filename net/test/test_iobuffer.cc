#include "iobuffer.h"

#include <string>

#include <cstdio>

using Buffer = net::IOBuffer;

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")


void testPush(const char *module, const char *msg, const char *data, Buffer &b, std::string &s, bool const expect = true){
	bool const result = b.push(data);

	if (expect)
		s += data;

	PRINTF_TEST(msg,	result == expect			);
	PRINTF_TEST("value",	s.compare(0, s.size(), b.data()) == 0	);

}


void testPop(const char *module, const char *msg, size_t const size, Buffer &b, std::string &s, bool const expect = true){
	bool const result = b.pop(size);

	if (expect)
		s.erase(0, size);

	PRINTF_TEST(msg, result == expect);

	if (s.empty()){
		PRINTF_TEST("size zero",	b.size() == 0				);
	}else{
		PRINTF_TEST("value",		s.compare(0, s.size(), b.data()) == 0	);
	}
}


void test(const char *module = "iobuffer"){
	Buffer b;

	std::string s;

	testPush(module, "push",		"aaa",	b, s);
	testPush(module, "push zero",		"",	b, s, false);
	testPush(module, "push",		"bbb",	b, s);
	testPush(module, "push",		"ccc",	b, s);

	testPop( module, "pop",			3,	b, s);
	testPop( module, "pop zero",		0,	b, s, false);
	testPop( module, "pop",			3,	b, s);
	testPop( module, "pop",			3,	b, s);
}

int main(){
	test();
}

