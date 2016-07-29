#include "iobuffer.h"

#include <string>

#include <cstdio>


#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")


template<class BUFFER>
void testPush(const char *module, const char *msg, const char *data, BUFFER &b, std::string &s, bool const expect = true){
	bool const result = b.push(data);

	if (expect)
		s += data;

	PRINTF_TEST(msg,	result == expect			);
	PRINTF_TEST("value",	s.compare(0, s.size(), b.data()) == 0	);

}


template<class BUFFER>
void testPop(const char *module, const char *msg, size_t const size, BUFFER &b, std::string &s, bool const expect = true){
	bool const result = b.pop(size);

	if (expect)
		s.erase(0, size);

	PRINTF_TEST(msg, result == expect);

	if (s.empty()){
		PRINTF_TEST("size zero",	b.size() == 0				);
		PRINTF_TEST("capacity",		b.capacity() == b.max_size()		);
	}else{
		PRINTF_TEST("value",		s.compare(0, s.size(), b.data()) == 0	);
	}
}


void test(const char *module = "iobuffer"){
	using Buffer = net::IOBuffer<8>;

	Buffer b;

	std::string s;

	testPush(module, "push",		"aaa",	b, s);
	testPush(module, "push zero",		"",	b, s, false);
	testPush(module, "push",		"bbb",	b, s);
	testPush(module, "push overflow",	"ccc",	b, s, false);
	testPush(module, "push limit",		"dd",	b, s);
	testPush(module, "push overflow",	"ccc",	b, s, false);

	testPop( module, "pop",			3,	b, s);
	testPop( module, "pop zero",		0,	b, s, false);
	testPop( module, "pop",			3,	b, s);
	testPop( module, "pop overflow",	3,	b, s, false);
	testPop( module, "pop limit",		2,	b, s);
	testPop( module, "pop overflow",	3,	b, s, false);
}

int main(){
	test();
}

