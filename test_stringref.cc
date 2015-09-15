#include "stringref.h"

#include <iostream>

#include <cstdio>
#include <cstring>

const char *module = "main";
const char *bla = "bla";
const char *hello = "hello";

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")

static void test_ref(const char *module, const StringRef &sr1){
	PRINTF_TEST("! empty",		! sr1.empty()				);

	PRINTF_TEST("size",		sr1.size() == strlen(hello)		);
	PRINTF_TEST("size",		sr1.length() == strlen(hello)		);

	PRINTF_TEST("data",		strcmp(sr1.data(), hello) == 0		);
	PRINTF_TEST("data",		strcmp(sr1.c_str(), hello) == 0		);

	PRINTF_TEST("cmp char *",	sr1.compare(hello) == 0			);
	PRINTF_TEST("cmp string",	sr1.compare(std::string(hello)) == 0	);
	PRINTF_TEST("cmp StringRef",	sr1.compare(StringRef(hello)) == 0	);

	PRINTF_TEST("cmp ___",		sr1.compare("_aaaa") > 0		);
	PRINTF_TEST("cmp ~~~",		sr1.compare("~aaaa") < 0		);

	PRINTF_TEST("== char *",	sr1 == hello				);
	PRINTF_TEST("== string",	sr1 == std::string(hello)		);
	PRINTF_TEST("== StringRef",	sr1 == StringRef(hello)			);

	PRINTF_TEST("|= char *",	sr1 != bla				);
	PRINTF_TEST("|= string",	sr1 != std::string(bla)			);
	PRINTF_TEST("|= StringRef",	sr1 != StringRef(bla)			);

	PRINTF_TEST("[] char *",	sr1[0] == hello[0]			);

	std::cout <<  sr1 << std::endl;
}

static void test_small_ref(const char *module){
	const StringRef sr1a = { hello, 1 };
	const char sr1b[] = { hello[0], '\0' };

	PRINTF_TEST("size",		sr1a.size() == 1			);

	PRINTF_TEST("eq char *",	sr1a == sr1b				);
	PRINTF_TEST("eq  string",	sr1a == std::string(sr1b)		);
	PRINTF_TEST("eq StringRef",	sr1a == StringRef(sr1b)		);
	PRINTF_TEST("eq char",		sr1a == sr1b[0]				);
}

static void test_string_ref(const char *module){
	const StringRef srs = hello;
	const std::string str = srs;

	PRINTF_TEST("string size",	str.size() == strlen(hello)		);
	PRINTF_TEST("string equal",	str == hello				);
}

int main(){
	StringRef sre;

	PRINTF_TEST("size",		sre.size() == 0				);
	PRINTF_TEST("data",		strcmp(sre.data(), "") == 0		);
	PRINTF_TEST("empty",		sre.empty()				);

	test_string_ref("string");

	test_small_ref("sm StringRef");

	test_ref("char *", hello);
	test_ref("string", std::string(hello));

	return 0;
}

