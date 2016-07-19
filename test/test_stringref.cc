#include "stringref.h"

#include <iostream>

#include <cstdio>
#include <cstring>

const char *module = "main";
const char *bla = "bla";
const char *hello = "hello";

constexpr const char bla_c[] = "bla";
constexpr const char hello_c[] = "hello";

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")

static void test_tools_empty_ref(const char *module);
static void test_tools_compare_ref(const char *module);

static void test_null_ref(const char *module);
static void test_small_ref(const char *module);
static void test_std_string_ref(const char *);

static void test_stringref(const char *module, const StringRef &sr);

int main(){
	test_tools_empty_ref("tools empty");
	test_tools_compare_ref("tools compare");

	test_null_ref("null");
	test_small_ref("small");
	test_std_string_ref("std::string");

	test_stringref("char *",	hello);
	test_stringref("char []",	hello_c);
	test_stringref("std::string",	std::string(hello));

	return 0;
}

// ==================================

static void test_tools_empty_ref(const char *module){
	const char *s = nullptr;

	PRINTF_TEST("empty",		StringRef::fastEmptyChar(s)				);
	PRINTF_TEST("! empty",		! StringRef::fastEmptyChar(hello)			);
	PRINTF_TEST("! empty",		! StringRef::fastEmptyChar(hello, strlen(hello))	);

	PRINTF_TEST("null",		StringRef::fastEmptyChar(s, 5)				);
	PRINTF_TEST("zero",		StringRef::fastEmptyChar(hello, 0)			);
	PRINTF_TEST("null zero",	StringRef::fastEmptyChar(s, 0)				);
}

static void test_tools_compare_ref(const char *module){
	PRINTF_TEST("null",		! StringRef::compare(	"", 0,			"", 0)			);

	PRINTF_TEST("<",		StringRef::compare(	"a", 1,			"b", 1) < 0		);
	PRINTF_TEST(">",		StringRef::compare(	"b", 1,			"a", 1) > 0		);

	PRINTF_TEST("eq",		! StringRef::compare(	hello, strlen(hello),	hello, strlen(hello))	);

	PRINTF_TEST("! eq",		StringRef::compare(	hello, 0,		hello, strlen(hello))	);
	PRINTF_TEST("! eq",		StringRef::compare(	hello, 1,		hello, strlen(hello))	);
	PRINTF_TEST("! eq",		StringRef::compare(	hello, strlen(hello),	hello, 0)		);
	PRINTF_TEST("! eq",		StringRef::compare(	hello, strlen(hello),	hello, 1)		);

}

// ==================================

static void test_null_ref(const char *module){
	StringRef sr;

	PRINTF_TEST("size",		sr.size() == 0				);
	PRINTF_TEST("data",		strcmp(sr.data(), "") == 0		);
	PRINTF_TEST("empty",		sr.empty()				);
}

static void test_small_ref(const char *module){
	const StringRef sr = { hello, 1 };
	const char ch[] = { hello[0], '\0' };

	PRINTF_TEST("size",		sr.size() == 1				);

	PRINTF_TEST("eq char *",	sr == ch				);
	PRINTF_TEST("eq  string",	sr == std::string(ch)			);
	PRINTF_TEST("eq StringRef",	sr == StringRef(ch)			);
	PRINTF_TEST("eq char",		sr == ch[0]				);
}

static void test_std_string_ref(const char *){
	const StringRef srs = hello;
	const std::string str = srs;

	PRINTF_TEST("size",		str.size() == strlen(hello)		);
	PRINTF_TEST("equal",		str == hello				);
}

// ==================================

static void test_stringref(const char *module, const StringRef &sr){
	PRINTF_TEST("! empty",		! sr.empty()				);

	PRINTF_TEST("size",		sr.size() == strlen(hello)		);
	PRINTF_TEST("size",		sr.length() == strlen(hello)		);

	PRINTF_TEST("data",		strcmp(sr.data(), hello) == 0		);
	PRINTF_TEST("data",		strcmp(sr.c_str(), hello) == 0		);

	PRINTF_TEST("cmp char *",	sr.compare(hello) == 0			);
	PRINTF_TEST("cmp char []",	sr.compare(hello_c) == 0		);
	PRINTF_TEST("cmp string",	sr.compare(std::string(hello)) == 0	);
	PRINTF_TEST("cmp StringRef",	sr.compare(StringRef(hello)) == 0	);

	PRINTF_TEST("cmp ___",		sr.compare("_aaaa") > 0			);
	PRINTF_TEST("cmp ~~~",		sr.compare("~aaaa") < 0			);

	PRINTF_TEST("== char *",	sr == hello				);
	PRINTF_TEST("== char []",	sr == hello_c				);
	PRINTF_TEST("== string",	sr == std::string(hello)		);
	PRINTF_TEST("== StringRef",	sr == StringRef(hello)			);

	PRINTF_TEST("|= char *",	sr != bla				);
	PRINTF_TEST("!= char []",	sr != bla_c				);
	PRINTF_TEST("|= string",	sr != std::string(bla)			);
	PRINTF_TEST("|= StringRef",	sr != StringRef(bla)			);

	PRINTF_TEST("[] char *",	sr[0] == hello[0]			);

	size_t i = 0;
	for(auto it = sr.begin(); it < sr.end(); ++it){
		PRINTF_TEST("iterator",	sr[i] == *it				);
		++i;
	}

	std::cout <<  sr << std::endl;
}





