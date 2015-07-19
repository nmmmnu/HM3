#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#include <utility>	// std::move

#include <stdio.h>	// printf
#include <string.h>	// strcmp

#define PRINTF_TEST(test, result) \
	printf("%-14s : Testing %-20s %s\n", module, test, result ? "OK" : "Fail")


inline static size_t list_add(IList &list, const Pair &p){
	list.put(p);
	return p.getSize();
}

static size_t list_populate(IList &list){
	list.removeAll();

	size_t size = 0;

	size += list_add(list, Pair("3 city",	"Sofia"	));
	size += list_add(list, Pair("1 name",	"Niki"	));
	size += list_add(list, Pair("4 os",	"Linux"	));
	size += list_add(list, Pair("2 age",	"22"	));

	return size;
}

static void list_test(const char *module, IList &list){
	Pair p = nullptr;

	// TEST GENERAL

	size_t size = list_populate(list);

	list.print();

	PRINTF_TEST("count",		list.getCount() == 4			);
	PRINTF_TEST("empty",		! list.isEmpty()			);
	PRINTF_TEST("sizeof",		list.getSize() == size			);



	// TEST GET

	p = list.get("3 city");
	PRINTF_TEST("get",		p && ! strcmp(p.getVal(), "Sofia")	);

	p = list["3 city"];
	PRINTF_TEST("get[]",		p && ! strcmp(p.getVal(), "Sofia")	);

	p = list.get("nonexistent");
	PRINTF_TEST("get non existent",	! p					);


	// TEST OVERWRITE

	const char *key_overwr = "2 val";
	const char *val_overwr = "overwritten";

	list.put( Pair(key_overwr, "original") );
	list.put( Pair(key_overwr, val_overwr) );
	p = list.get(key_overwr);
	PRINTF_TEST("overwrite",	p && ! strcmp(p.getVal(), val_overwr)	);



	// TEST REMOVE

	list_populate(list);

	// remove non existent
	list.remove("nonexistent");

	// remove middle
	list.remove("2 age");

	// remove first
	list.remove("1 name");

	// remove last
	list.remove("4 os");

	p = list.get("3 city");

	PRINTF_TEST("remove",		p && ! strcmp(p.getVal(), "Sofia")	);
	PRINTF_TEST("remove count",	list.getCount() == 1			);
	PRINTF_TEST("remove sizeof",	list.getSize() == p.getSize()		);

	// overwrite sizeof test
	const Pair sopair = {"3 city", nullptr};
	list.put(sopair);
	PRINTF_TEST("overwrite count",	list.getCount() == 1			);
	PRINTF_TEST("overwrite sizeof",	list.getSize() == sopair.getSize()	);

	// remove last element
	list.remove("3 city");

	// remove non existent from empty list
	list.remove("nonexistent");

	PRINTF_TEST("remove count",	list.getCount() == 0			);
	PRINTF_TEST("remove empty",	list.isEmpty()				);



	// TEST ITERATOR

	list_populate(list);

	p = list.first();
	PRINTF_TEST("it first",		p && strcmp(p.getVal(), "Niki") == 0	);
	p = list.current();                          
	PRINTF_TEST("it current",	p && strcmp(p.getVal(), "Niki") == 0	);
                                                     
	p = list.first("2 age");                     
	PRINTF_TEST("it first",		p && strcmp(p.getVal(), "22") == 0	);
	p = list.current();                          
	PRINTF_TEST("it current",	p && strcmp(p.getVal(), "22") == 0	);
	p = list.next();                             
	PRINTF_TEST("it next",		p && strcmp(p.getVal(), "Sofia") == 0	);
	p = list.current();                          
	PRINTF_TEST("it current",	p && strcmp(p.getVal(), "Sofia") == 0	);
                                                     
	p = list.first("4 os");                      
	PRINTF_TEST("it first",		p && strcmp(p.getVal(), "Linux") == 0	);
	p = list.current();                          
	PRINTF_TEST("it current",	p && strcmp(p.getVal(), "Linux") == 0	);
	p = list.next();
	PRINTF_TEST("it next",		! p					);
	p = list.current();
	PRINTF_TEST("it current",	! p					);

	p = list.first("5");
	PRINTF_TEST("it next it",	! p					);

	p = list.first();
	auto version = list.getVersion();
	PRINTF_TEST("it invalidate 1",	p					);
	list.remove("1 name");
	p = list.next();
	PRINTF_TEST("it invalidate 2",	! p					);

	PRINTF_TEST("invalidate ver",	version < list.getVersion()		);

}

static void skiplist_lanes_test(SkipList &list){
	list.put(Pair("name",		"Niki"		));
	list.put(Pair("city",		"Sofia"		));
	list.put(Pair("state",		"na"		));
	list.put(Pair("zip",		"1000"		));
	list.put(Pair("country",	"BG"		));
	list.put(Pair("phone",		"+358 888 1000"	));
	list.put(Pair("fax",		"+358 888 2000"	));
	list.put(Pair("email",		"user@aol.com"	));
	list.put(Pair("laptop",		"Dell"		));
	list.put(Pair("os",		"Archlinux"	));
	list.put(Pair("mouse",		"Logitech"	));

	list.printLanes();

//	list.print();
}

int main(int argc, char **argv){

	VectorList vl;
		list_test("Vector", vl);

	VectorList vl2 = { std::move(vl) };
		list_test("Moved Vector", vl2);

	// =========================

	LinkList ll;
		list_test("LinkList", ll);

	LinkList ll2 = { std::move(ll) };
		list_test("Moved LinkList", ll2);

	// =========================

	SkipList sl;
		list_test("SkipList", sl);
		if (0)
			skiplist_lanes_test(sl);

	SkipList sl2 = { std::move(sl) };
		list_test("Moved SkipList", sl2);

	// =========================

	return 0;
}

