#include "pair.h"
#include "nmea0183checksumcalculator.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp
#include <unistd.h>	// sleep

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

	Pair *p = (Pair *) raw_memory;

	PRINTF_TEST("raw key",		strcmp(p->getKey(), key) == 0		);
	PRINTF_TEST("raw val",		strcmp(p->getVal(), val) == 0		);

	PRINTF_TEST("raw cmp",		p->cmp(key) == 0			);
	PRINTF_TEST("raw cmp",		p->cmp("~~~ non existent") < 0		);
	PRINTF_TEST("raw cmp",		p->cmp("!!! non existent") > 0		);

}

static void pair_test(){
	const char *key = "abcdef";
	const char *val = "1234567890";

	IChecksumCalculator *cs = new NMEA0183ChecksumCalculator();
	Pair::setChecksumCalculator(cs);

	Pair *p = Pair::create(key, val);

	Pair *t = Pair::create(key);

	PRINTF_TEST("tombstone",	t->getVal() == NULL			);

	PRINTF_TEST("key",		strcmp(p->getKey(), key) == 0		);
	PRINTF_TEST("val",		strcmp(p->getVal(), val) == 0		);

	PRINTF_TEST("cmp",		p->cmp(key) == 0			);
	PRINTF_TEST("cmp",		p->cmp("~~~ non existent") < 0		);
	PRINTF_TEST("cmp",		p->cmp("!!! non existent") > 0		);

	PRINTF_TEST("cmp class",	p->cmp(t) == 0				);

	PRINTF_TEST("cmp null",		p->cmp((char *)NULL)			);
	PRINTF_TEST("cmp null",		p->cmp((Pair *)NULL)			);

	// these always pass
	PRINTF_TEST("valid",		p->valid()				);

	char *corruptor = (char *)p->getKey();
	corruptor[0] = ~ corruptor[0];

	PRINTF_TEST("valid corrupted",	! p->valid()				);

	Pair::setChecksumCalculator(NULL);
	PRINTF_TEST("valid null",	p->valid()				);

	corruptor[0] = ~ corruptor[0];

	p->print();
	t->print();

	Pair::destroy(p);
	Pair::destroy(t);

	delete cs;
}

__attribute__ ((unused))
static void pair_test_delay(){
	Pair *p = Pair::create("test", 1);

	PRINTF_TEST("not expired",	p->valid()				);
	printf("sleep for 1 sec...\n");
	sleep(2);
	PRINTF_TEST("expired",		! p->valid()				);

	Pair::destroy(p);
}

int main(int argc, char **argv){
	pair_test_raw();
	pair_test();
	pair_test_delay();

	return 0;
}

