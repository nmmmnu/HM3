#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"
#include "nullsafelist.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp

#define MODULE_NAME	"List"

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", MODULE_NAME, test, result ? "OK" : "Fail")


static size_t list_populate(IList *list){
	list->removeAll();

	Pair *p;

	size_t size = 0;

	p = Pair::create("3 city",	"Sofia"	);	size += p->getSize();	list->put(p);
	p = Pair::create("1 name",	"Niki"	);	size += p->getSize();	list->put(p);
	p = Pair::create("4 os",	"Linux"	);	size += p->getSize();	list->put(p);
	p = Pair::create("2 age",	"22"	);	size += p->getSize();	list->put(p);

	return size;
}

static void list_test(IList *list){
	const Pair *p;

	size_t size = list_populate(list);

	list->getIterator()->print();

	PRINTF_TEST("count",		list->getCount() == 4			);
	PRINTF_TEST("sizeof",		list->getSize() == size			);

	// testing overwrite

	const char *key_overwr = "2 val";
	const char *val_overwr = "overwritten";

	list->put( Pair::create(key_overwr, "original")	);
	list->put( Pair::create(key_overwr, val_overwr)	);
	p = list->get(key_overwr);

	if (p)
	PRINTF_TEST("overwrite",	! strcmp(p->getVal(), val_overwr)	);

	// testing remove
	list_populate(list);

	// remove non existent
	list->remove("nonexistent");

	// remove middle
	list->remove("2 age");

	// remove first
	list->remove("1 name");

	// remove middle
	list->remove("4 os");

	p = list->get("3 city");

	if (p)
	PRINTF_TEST("remove",		! strcmp(p->getVal(), "Sofia")		);
	PRINTF_TEST("remove count",	list->getCount() == 1			);
	if (p)
	PRINTF_TEST("remove sizeof",	list->getSize() == p->getSize()		);

	// remove last
	list->remove("3 city");

	// remove non existent
	list->remove("nonexistent");

	PRINTF_TEST("remove count",	list->getCount() == 0			);

	delete list;
}

__attribute__ ((unused))
static void skiplist_test(SkipList *list){
	list->put(Pair::create("name",		"Niki"		));
	list->put(Pair::create("city",		"Sofia"		));
	list->put(Pair::create("state",		"na"		));
	list->put(Pair::create("zip",		"1000"		));
	list->put(Pair::create("country",	"BG"		));
	list->put(Pair::create("phone",		"+358 888 1000"	));
	list->put(Pair::create("fax",		"+358 888 2000"	));
	list->put(Pair::create("email",		"user@aol.com"	));

	list->put(Pair::create("laptop",	"Dell"		));
	list->put(Pair::create("os",		"Archlinux"	));
	list->put(Pair::create("mouse",		"Logitech"	));

	list->printLanes();

	list->getIterator()->print();

	delete list;
}

int main(int argc, char **argv){
	list_test( new VectorList()	);
	list_test( new LinkList()	);
	list_test( new SkipList()	);
	//skiplist_test( new SkipList()	);
	list_test( new NULLSafeList(new SkipList())	);

	return 0;
}

