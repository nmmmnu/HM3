#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#include <utility>	// std::move

#include <stdio.h>	// printf
#include <string.h>	// strcmp

#define PRINTF_TEST(test, result) \
	printf("%-14s : Testing %-20s %s\n", module, test, result ? "OK" : "Fail")



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



template <class LIST>
inline static size_t list_add(LIST &list, Pair &&p){
	list.put(p);
	return p.getSize();
}

template <class LIST>
static size_t list_populate(LIST &list){
	list.removeAll();

	size_t size = 0;

	size += list_add(list, {"3 city",	"Sofia"	});
	size += list_add(list, {"1 name",	"Niki"	});
	size += list_add(list, {"4 os",		"Linux"	});
	size += list_add(list, {"2 age",	"22"	});

	return size;
}



template <class LIST>
static void ref_test(const char *module, LIST &list){
	const char *key = "ref_test";

	const Pair p = Pair::tombstone(key);
	p.print();

	list.put(p);

	p.print();

	list.get(key).print();

	p.print();

	list.remove(key);

	p.print();

	PRINTF_TEST("ref test",		true					);
}

template <class IT>
inline void iterator_test_deref(const char *module, IT &it, IT &et, const char *value){
	PRINTF_TEST("*it   deref",	it != et && (*it).getVal() == value	);
	PRINTF_TEST(" it-> deref",	it != et && it->getVal() == value	);
	++it;
}

template <class LIST>
static void iterator_test(const char *module, LIST &list){
	auto it = list.begin();
	auto et = list.end();

	iterator_test_deref(module, it, et, "Niki"	);
	iterator_test_deref(module, it, et, "22"	);
	iterator_test_deref(module, it, et, "Sofia"	);
	iterator_test_deref(module, it, et, "Linux"	);

	PRINTF_TEST("*it end()",	it == et				);
}

template <class LIST>
static void list_test(const char *module, LIST &list){
	Pair p = nullptr;



	// TEST GENERAL

	size_t size = list_populate(list);

	PRINTF_TEST("print it",		true					);
	list.print();

	PRINTF_TEST("count",		list.getCount() == 4			);
	PRINTF_TEST("empty",		! list.isEmpty()			);
	PRINTF_TEST("sizeof",		list.getSize() == size			);



	// TEST GET

	p = list.get("3 city");
	PRINTF_TEST("get",		p && p.getVal() == "Sofia"		);

//	p = list["3 city"];
//	PRINTF_TEST("get[]",		p && p.getVal() == "Sofia"		);

	p = list.get("nonexistent");
	PRINTF_TEST("get non existent",	! p					);



	// TEST OVERWRITE

	const char *key_overwr = "2 val";
	const char *val_overwr = "overwritten";

	list.put( Pair(key_overwr, "original") );
	list.put( Pair(key_overwr, val_overwr) );
	p = list.get(key_overwr);
	PRINTF_TEST("overwrite",	p && p.getVal() == val_overwr		);



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

	PRINTF_TEST("remove",		p && p.getVal() == "Sofia"		);
	PRINTF_TEST("remove count",	list.getCount() == 1			);
	PRINTF_TEST("remove sizeof",	list.getSize() == p.getSize()		);

	// overwrite sizeof test
	const Pair sopair = { "3 city", "" };

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

	iterator_test(module, list);

	// TEST REFERENCES

	list.removeAll();
	ref_test(module, list);

	// MOVE TESTS

	size = list_populate(list);

	LIST mlist = std::move(list);
	PRINTF_TEST("move c-tor 1",	mlist.getSize() == size			);
	PRINTF_TEST("move c-tor 2",	list.isEmpty()				);
}

int main(int argc, char **argv){

	VectorList vl;
		list_test("Vector", vl);

	// =========================

	LinkList ll;
		list_test("LinkList", ll);

	// =========================

	SkipList sl;
		list_test("SkipList", sl);
		if (0)
			skiplist_lanes_test(sl);

	// =========================

	return 0;
}


