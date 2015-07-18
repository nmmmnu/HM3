#include "pair.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp
#include <unistd.h>	// sleep

#include <utility>	// std::move

#include "nmea0183checksumcalculator.h"

#define MODULE_NAME	"Pair"

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", MODULE_NAME, test, result ? "OK" : "Fail")

static void pair_test_raw(){
	const char *key = "name";
	const char *val = "Peter";

	static char raw_memory[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,	// created
		0x00, 0x00, 0x00, 0x00,				// expires
		0x00, 0x00, 0x00, 0x05,				// vallen
		0x00, 0x04,					// keylen
		0x00,						// checksum
		'n', 'a', 'm', 'e', '\0',			// key
		'P', 'e', 't', 'e', 'r', '\0'			// val
	};

	Pair p = (const void *) raw_memory;

	p.print();

	PRINTF_TEST("raw key",		strcmp(p.getKey(), key) == 0	);
	PRINTF_TEST("raw val",		strcmp(p.getVal(), val) == 0	);

	PRINTF_TEST("raw cmp",		p.cmp(key) == 0			);
	PRINTF_TEST("raw cmp",		p.cmp("~~~ non existent") < 0	);
	PRINTF_TEST("raw cmp",		p.cmp("!!! non existent") > 0	);
	
	Pair p2 = p;
	PRINTF_TEST("copy c-tor buff",	p.getBlob() == p2.getBlob()	);

	p2.print();
	
	p2 = p;
	PRINTF_TEST("copy assign buff",	p.getBlob() == p2.getBlob()	);
	
	p2.print();
}

static void pair_test_null(){
	Pair p = nullptr;

	p.print();

	PRINTF_TEST("null bool",	p == false			);
	PRINTF_TEST("null key",		p.getKey() == nullptr		);
	PRINTF_TEST("null val",		p.getVal() == nullptr		);
	PRINTF_TEST("null cmp",		p.cmp("bla") == 1		);
}

static void pair_test(){
	const char *key = "abcdef";
	const char *val = "1234567890";

	NMEA0183ChecksumCalculator cs = NMEA0183ChecksumCalculator();
	Pair::setChecksumCalculator(cs);

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
	char *corruptor = (char *)p.getKey();
	corruptor[0] = ~ corruptor[0];

	PRINTF_TEST("valid corrupted",	! p.valid()			);

	Pair::removeChecksumCalculator();
	PRINTF_TEST("valid null",	p.valid()			);

	corruptor[0] = ~ corruptor[0];
	}

	{
	Pair m1 = { key, val };
	Pair m2 = std::move(m1);
	PRINTF_TEST("move c-tor",	strcmp(m2.getKey(), key) == 0	);
	Pair m3 = m2;	
	PRINTF_TEST("copy c-tor",	strcmp(m2.getKey(), key) == 0	);
	PRINTF_TEST("copy c-tor",	strcmp(m3.getKey(), key) == 0	);
	
	PRINTF_TEST("copy c-tor buff",	m2.getBlob() != m3.getBlob()	);
	
	m1 = m2;
	PRINTF_TEST("copy assign",	strcmp(m1.getKey(), key) == 0	);
	PRINTF_TEST("copy assign buff",	m2.getBlob() != m3.getBlob()	);
	}
	
	p.print();
	t.print();
}

static void pair_test_expired(bool sl = false){
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

int main(int argc, char **argv){
	pair_test_raw();
	pair_test_null();
	pair_test();
	pair_test_expired();

	return 0;
}

