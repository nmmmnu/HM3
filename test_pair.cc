#include "pair.h"
#include "nmea0183checksumcalculator.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp
#include <unistd.h>	// sleep

#define MODULE_NAME	"Pair"

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", MODULE_NAME, test, result ? "OK" : "Fail")



static void pair_test(){
	const char *key = "aaancdefgh";
	const char *val = "1234567890";

	IChecksumCalculator *cs = new NMEA0183ChecksumCalculator();

	Pair *p = new Pair(cs);
	p->create(key, val);

	Pair *p2 = new Pair();
	p2->create(key);

	PRINTF_TEST("key",		strcmp(p->getKey(), key) == 0		);
	PRINTF_TEST("val",		strcmp(p->getVal(), val) == 0		);

	PRINTF_TEST("tombstone",	p2->getVal() == NULL			);

	PRINTF_TEST("cmp",		p->cmp(key) == 0			);
	PRINTF_TEST("cmp",		p->cmp("~~~ non existent") < 0		);
	PRINTF_TEST("cmp",		p->cmp("!!! non existent") > 0		);

	// these always pass
	PRINTF_TEST("valid",		p->valid()				);
	// p2 IChecksumCalculator is NULL
	PRINTF_TEST("valid",		p2->valid()				);

	p->print();
	p2->print();

	delete p;
	delete p2;
	delete cs;
}

static void pair_test_delay(){
	Pair *p = new Pair();
	p->create("test", 1);

	PRINTF_TEST("expiration",	p->valid()				);
	printf("sleep for 1 sec...\n");
	sleep(2);
	PRINTF_TEST("expiration",	! p->valid()				);

	p->print();

	delete p;
}

int main(int argc, char **argv){
	pair_test();
	pair_test_delay();

	return 0;
}

