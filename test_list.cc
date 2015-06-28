#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"
#include "nullsafelist.h"

#include <stdio.h>	// printf
#include <string.h>	// strcmp

#define MODULE_NAME	"List"

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", MODULE_NAME, test, result ? "OK" : "Fail")


inline static size_t list_add(IList &list, IPair &p){
	list.put(p);
	return p.getSize();
}

static size_t list_populate(IList &list){
	list.removeAll();

	size_t size = 0;

	{ Pair p = {"3 city",	"Sofia"	}; size += list_add(list, p); }
	{ Pair p = {"1 name",	"Niki"	}; size += list_add(list, p); }
	{ Pair p = {"4 os",	"Linux"	}; size += list_add(list, p); }
	{ Pair p = {"2 age",	"22"	}; size += list_add(list, p); }

	return size;
}

static void list_test(IList &list){
	size_t size = list_populate(list);

	list.print();

	PRINTF_TEST("count",		list.getCount() == 4			);
	PRINTF_TEST("empty",		! list.isEmpty()			);
	PRINTF_TEST("sizeof",		list.getSize() == size			);

	// testing get

	Pair p = list.get("3 city");
	if (p)
	PRINTF_TEST("get",		! strcmp(p.getVal(), "Sofia")	);

	p = list["3 city"];
	if (p)
	PRINTF_TEST("get",		! strcmp(p.getVal(), "Sofia")	);

	p = list.get("nonexistent");
	PRINTF_TEST("get non existent",	! p					);

	// testing overwrite

	const char *key_overwr = "2 val";
	const char *val_overwr = "overwritten";

	Pair pp1{key_overwr, "original"};
	list.put( pp1	);
	Pair pp2{key_overwr, val_overwr};
	list.put( pp2	);
	p = list.get(key_overwr);

	if (p)
	PRINTF_TEST("overwrite",	! strcmp(p.getVal(), val_overwr)	);

	// testing remove
	list_populate(list);

	// remove non existent
	list.remove("nonexistent");

	// remove middle
	list.remove("2 age");

	// remove first
	list.remove("1 name");

	// remove middle
	list.remove("4 os");

	p = list.get("3 city");

	if (p)
	PRINTF_TEST("remove",		! strcmp(p.getVal(), "Sofia")		);
	PRINTF_TEST("remove count",	list.getCount() == 1			);
	if (p)
	PRINTF_TEST("remove sizeof",	list.getSize() == p.getSize()		);

	// overwrite sizeof test
	Pair sopair = {"3 city", nullptr};
	list.put(sopair);
	PRINTF_TEST("overwrite count",	list.getCount() == 1			);
	PRINTF_TEST("overwrite sizeof",	list.getSize() == sopair.getSize()	);

	// remove last
	list.remove("3 city");

	// remove non existent
	list.remove("nonexistent");

	PRINTF_TEST("remove count",	list.getCount() == 0			);
	PRINTF_TEST("remove empty",	list.isEmpty()				);

	// testing iterator
	list_populate(list);

	PRINTF_TEST("iterator rewind",	list.rewind()				);
	PRINTF_TEST("iterator rewind",	! list.rewind("bla")			);
	PRINTF_TEST("iterator rewind",	list.rewind("2 age")			);
	p = list.next();
	PRINTF_TEST("iterator next",	strcmp(p.getVal(), "22") == 0	);
	p = list.next();
	PRINTF_TEST("iterator next",	strcmp(p.getVal(), "Sofia") == 0	);
}

static void skiplist_lanes_test(SkipList &list){
	{ Pair p = {"name",		"Niki"		}; list.put(p); }
	{ Pair p = {"city",		"Sofia"		}; list.put(p); }
	{ Pair p = {"state",		"na"		}; list.put(p); }
	{ Pair p = {"zip",		"1000"		}; list.put(p); }
	{ Pair p = {"country",		"BG"		}; list.put(p); }
	{ Pair p = {"phone",		"+358 888 1000"	}; list.put(p); }
	{ Pair p = {"fax",		"+358 888 2000"	}; list.put(p); }
	{ Pair p = {"email",		"user@aol.com"	}; list.put(p); }

	{ Pair p = {"laptop",		"Dell"		}; list.put(p); }
	{ Pair p = {"os",		"Archlinux"	}; list.put(p); }
	{ Pair p = {"mouse",		"Logitech"	}; list.put(p); }

	list.printLanes();

//	list.print();
}

static void skiplist_test(SkipList &list, bool lanes){
	if (lanes)
		skiplist_lanes_test(list);

	list_test(list);
}

static void nullsafelist_test(NULLSafeList &&list){
	PRINTF_TEST("get null",		! list.get(NULL)		);
	PRINTF_TEST("remove null",	! list.remove(NULL)		);
}

int main(int argc, char **argv){
	VectorList vl;
	list_test(vl);

	LinkList ll;
	list_test(ll);

	SkipList sl;
	skiplist_test(sl, false);

	nullsafelist_test( NULLSafeList{vl} );

	return 0;
}

