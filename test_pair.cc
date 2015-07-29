#include "pair.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp
#include <unistd.h>	// sleep

#include <utility>	// std::move

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")



static void pair_test_raw(const char *module		= "raw pair"	);
static void pair_test_null(const char *module		= "null pair"	);
static void pair_test(const char *module		= "pair"	);
static void pair_test_expired(const char *module	= "expired pair", bool sl = false);



int main(int argc, char **argv){
	pair_test_null();
	pair_test_raw();
	pair_test();
	pair_test_expired();

	return 0;
}



static void pair_test_raw_do(const char *module, Pair & p, const char *key, const char *val){
	p.print();

	PRINTF_TEST("valid",		p.valid()			);

	PRINTF_TEST("key",		strcmp(p.getKey(), key) == 0	);
	PRINTF_TEST("val",		strcmp(p.getVal(), val) == 0	);

	PRINTF_TEST("cmp",		p.cmp(key) == 0			);
	PRINTF_TEST("cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("cmp",		p.cmp("!!! non existent") > 0	);

	Pair p2 = p;

	p2.print();

	p2 = p;

	p2.print();
}

static void pair_test_raw(const char *module){
	const char *key = "name";
	const char *val = "Peter";

	NMEA0183ChecksumCalculator nm;

	char checksum = (char) nm.calc("name\0Peter\0", 11);

	static char raw_memory[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,	// created
		0x00, 0x00, 0x00, 0x00,				// expires
		0x00, 0x00, 0x00, 0x05,				// vallen
		0x00, 0x04,					// keylen
		checksum,					// checksum
		'n', 'a', 'm', 'e', '\0',			// key
		'P', 'e', 't', 'e', 'r', '\0'			// val
	};

	Pair p = { (const void *) raw_memory, true };
	pair_test_raw_do("weak pair", p, key, val);

	p = (const void *) raw_memory;
	pair_test_raw_do(module, p, key, val);

	raw_memory[20] = ~ raw_memory[20];

	Pair cp = (const void *) raw_memory;

	PRINTF_TEST("valid corrupted",	! cp.valid()			);
}



static void pair_test_null(const char *module){
	Pair p = nullptr;

	p.print();

	PRINTF_TEST("null bool",	p == false			);
	PRINTF_TEST("null key",		p.getKey() == nullptr		);
	PRINTF_TEST("null val",		p.getVal() == nullptr		);
	PRINTF_TEST("null cmp",		p.cmp("bla") == 1		);
}



static void pair_test(const char *module){
	const char *key = "abcdef";
	const char *val = "1234567890";

	const Pair p = { key, val };

	const Pair t = { key, nullptr };

	PRINTF_TEST("null bull ok",	p == true			);
	PRINTF_TEST("tombstone",	t.getVal() == nullptr		);

	PRINTF_TEST("key",		strcmp(p.getKey(), key) == 0	);
	PRINTF_TEST("val",		strcmp(p.getVal(), val) == 0	);

	PRINTF_TEST("cmp",		p.cmp(key) == 0			);
	PRINTF_TEST("cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("cmp",		p.cmp("!!! non existent") > 0	);

	PRINTF_TEST("cmp",		p.cmp(t) == 0			);

	PRINTF_TEST("cmp null",		p.cmp((char *) nullptr)		);

	PRINTF_TEST("valid",		p.valid()			);
	PRINTF_TEST("valid",		p.valid(t)			);

	{
	Pair m1 = { key, val };
	Pair m2 = std::move(m1);
	PRINTF_TEST("move c-tor",	strcmp(m2.getKey(), key) == 0	);
	Pair m3 = m2;
	PRINTF_TEST("copy c-tor",	strcmp(m2.getKey(), key) == 0	);
	PRINTF_TEST("copy c-tor",	strcmp(m3.getKey(), key) == 0	);

	m1 = m2;
	PRINTF_TEST("copy assign",	strcmp(m1.getKey(), key) == 0	);
	}

	p.print();
	t.print();
}



static void pair_test_expired(const char *module, bool sl){
	Pair p1 = { "key", "val", 1 };

	PRINTF_TEST("not expired",	p1.valid()			);

	if (sl){
		printf("sleep for 2 sec...\n");
		sleep(2);
		PRINTF_TEST("expired",		! p1.valid()			);
	}

	Pair p2 = { "key", "val", 1, 3600 * 24 /* 1970-01-02 */ };
	PRINTF_TEST("expired",		! p2.valid()			);
}


