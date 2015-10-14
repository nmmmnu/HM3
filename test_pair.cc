#include "pair.h"
#include "pairpod.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp
#include <unistd.h>	// sleep

#include <utility>	// std::move

//#include "nmea0183checksumcalculator.h"

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



static void pair_test_raw_do(const char *module, const Pair & p, const StringRef &key, const StringRef &val){
	p.print();

	PRINTF_TEST("valid",		p.valid()			);

	PRINTF_TEST("key",		key == p.getKey()		);
	PRINTF_TEST("val",		val == p.getVal()		);

	PRINTF_TEST("cmp key",		p.cmp(key) == 0			);
	PRINTF_TEST("cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("cmp",		p.cmp("!!! non existent") > 0	);

	Pair p2 = p;

	p2.print();

	p2 = p;

	p2.print();
}

static void pairpod_test(const char *module, const PairPOD & p, const StringRef &key, const StringRef &val){
	PRINTF_TEST("valid",		p.valid()			);

	PRINTF_TEST("key",		key == p.getKey()		);
	PRINTF_TEST("val",		val == p.getVal()		);

	PRINTF_TEST("cmp key",		p.cmp(key.data()) == 0		);
	PRINTF_TEST("cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("cmp",		p.cmp("!!! non existent") > 0	);
}

static void pair_test_raw(const char *module){
	const char *key = "name";
	const char *val = "Peter";

	Checksum<NMEA0183ChecksumCalculator> chk;

	char checksum = (char) chk.calc("name\0Peter\0", 11);

	static char raw_memory[] = {
		0x50, 0x00, 0x00, 0x00,		// created, 2012-07-13 11:01:20
		0x00, 0x00, 0x00, 0x00,		// milliseconds
		0x00, 0x00, 0x00, 0x00,		// expires
		0x00, 0x00, 0x00, 0x05,		// vallen
		0x00, 0x04,			// keylen
		checksum,			// checksum
		'n', 'a', 'm', 'e', '\0',	// key
		'P', 'e', 't', 'e', 'r', '\0'	// val
	};

	auto pp = (const PairPOD *) raw_memory;
	pairpod_test("pod pair", *pp, key, val);

	const Pair p = (const void *) raw_memory;
	pair_test_raw_do(module, p, key, val);

	raw_memory[20] = ~ raw_memory[20];

	const Pair cp = (const void *) raw_memory;

	PRINTF_TEST("valid corrupted",	! cp.valid()			);
}



static void pair_test_null(const char *module){
	const Pair p = nullptr;

	p.print();

	PRINTF_TEST("null bool",	p == false			);
	PRINTF_TEST("null key",		p.getKey().empty()		);
	PRINTF_TEST("null val",		p.getVal().empty()		);
	PRINTF_TEST("null cmp",		p.cmp("bla") == 1		);
}



static void pair_test(const char *module){
	const char *key = "abcdef";
	const char *val = "1234567890";

	const Pair p = { key, val };

	const Pair t = Pair::tombstone(key);

	PRINTF_TEST("null bull ok",	p == true			);
	PRINTF_TEST("tombstone",	t.isTombstone()			);

	PRINTF_TEST("key",		p.getKey() == key		);
	PRINTF_TEST("val",		p.getVal() == val		);

	PRINTF_TEST("cmp",		p.cmp(key) == 0			);
	PRINTF_TEST("cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("cmp",		p.cmp("!!! non existent") > 0	);

	PRINTF_TEST("cmp",		p.cmp(t) == 0			);

	PRINTF_TEST("cmp null",		p.cmp((char *) nullptr)		);

	PRINTF_TEST("valid",		p.valid()			);
	PRINTF_TEST("valid",		p.valid(t)			);

	{
	Pair m1 = { key, val };

	const Pair m2 = std::move(m1);
	PRINTF_TEST("move c-tor",	m2.getKey() == key		);

	const Pair m3 = m2;
	PRINTF_TEST("copy c-tor",	m2.getKey() == key		);
	PRINTF_TEST("copy c-tor",	m3.getKey() == key		);

	m1 = m2;
	PRINTF_TEST("copy assign",	m1.getKey() == key		);
	}

	p.print();
	t.print();
}



static void pair_test_expired(const char *module, bool sl){
	const Pair p1 = { "key", "val", 1 };

	PRINTF_TEST("not expired",	p1.valid()			);

	if (sl){
		printf("sleep for 2 sec...\n");
		sleep(2);
		PRINTF_TEST("expired",		! p1.valid()		);
	}

	const Pair p2 = { "key", "val", 1, 3600 * 24 /* 1970-01-02 */ };
	PRINTF_TEST("expired",		! p2.valid()			);
}


