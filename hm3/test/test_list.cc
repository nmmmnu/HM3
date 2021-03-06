#include "stlvectorlist.h"
#include "vectorlist.h"
#include "linklist.h"
#include "skiplist.h"

#include "hashlist.h"

#include "blackholelist.h"

using Pair	= hm3::Pair;

#include <cstdio>	// printf
#include <cstring>	// strcmp
#include <vector>

#define PRINTF_TEST(test, result) \
	printf("%-20s : Testing %-20s %s\n", module, test, result ? "OK" : "Fail")


static void skiplist_lanes_test(hm3::SkipList &list) __attribute__((unused));

static void skiplist_lanes_test(hm3::SkipList &list){
	list.emplace("name",		"Niki"		);
	list.emplace("city",		"Sofia"		);
	list.emplace("state",		"na"		);
	list.emplace("zip",		"1000"		);
	list.emplace("country",		"BG"		);
	list.emplace("phone",		"+358 888 1000"	);
	list.emplace("fax",		"+358 888 2000"	);
	list.emplace("email",		"user@aol.com"	);
	list.emplace("laptop",		"Dell"		);
	list.emplace("os",		"Archlinux"	);
	list.emplace("mouse",		"Logitech"	);

	list.printLanes();

//	list.print();
}



template <class LIST>
inline static size_t list_add(LIST &list, Pair &&p){
	const size_t size = p.getBytes();
	list.put(std::move(p));
	return size;
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
	PRINTF_TEST(" it-> deref",	it != et &&   it->getVal() == value	);

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
inline void iterator_get_test_deref(const char *module, const LIST &list, const char *key, const char *value){
	auto it = list.lowerBound(key);
	auto et = list.end();

	if (value){
	PRINTF_TEST("*get it   deref",	it != et && (*it).getVal() == value	);
	PRINTF_TEST(" get it-> deref",	it != et &&   it->getVal() == value	);
	}else{
	PRINTF_TEST(".get it et",	it == et				);
	}
}

template <class LIST>
static void iterator_get_test(const char *module, LIST &list){
	iterator_get_test_deref(module, list, "1",	"Niki"	);
	iterator_get_test_deref(module, list, "1 name",	"Niki"	);
	iterator_get_test_deref(module, list, "2",	"22"	);
	iterator_get_test_deref(module, list, "2 age",	"22"	);
	iterator_get_test_deref(module, list, "3",	"Sofia"	);
	iterator_get_test_deref(module, list, "3 city",	"Sofia"	);
	iterator_get_test_deref(module, list, "4",	"Linux"	);
	iterator_get_test_deref(module, list, "4 os",	"Linux"	);
	iterator_get_test_deref(module, list, "5",	nullptr	);
	iterator_get_test_deref(module, list, "6",	nullptr	);

	iterator_get_test_deref(module, list, "",	nullptr	);
}

template <class LIST>
static void list_test(const char *module, LIST &list){
	Pair p = nullptr;
	StringRef nullstr = nullptr;


	PRINTF_TEST("put null",		list.put(p)				);

	PRINTF_TEST("get null",		list.get(nullstr).getKey().empty()	);
	PRINTF_TEST("remove null",	list.remove(nullstr)			);


	// TEST GENERAL

	const size_t size = list_populate(list);

	const typename LIST::size_type count = 4;

	PRINTF_TEST("print it",		true					);
	list.print();

	PRINTF_TEST("count",		list.getCount() == count		);
	PRINTF_TEST("count estim",	list.getCount(true) >= count		);
	PRINTF_TEST("empty",		! list.isEmpty()			);
	PRINTF_TEST("sizeof",		list.getBytes() == size			);



	// TEST GET

	p = list.get("3 city");
	PRINTF_TEST("get",		p.getVal() == "Sofia"			);

//	p = list["3 city"];
//	PRINTF_TEST("get[]",		p.getVal() == "Sofia"			);

	p = list.get("nonexistent");
	PRINTF_TEST("get non existent",	! p					);



	// TEST OVERWRITE

	const char *key_overwr = "2 val";
	const char *val_overwr = "overwritten";

	list.put( Pair(key_overwr, "original") );
	list.put( Pair(key_overwr, val_overwr) );
	p = list.get(key_overwr);
	PRINTF_TEST("overwrite",	p.getVal() == val_overwr		);



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

	PRINTF_TEST("remove",		p.getVal() == "Sofia"			);
	PRINTF_TEST("remove count",	list.getCount() == 1			);
	PRINTF_TEST("remove sizeof",	list.getBytes() == p.getBytes()		);

	// overwrite sizeof test
	const Pair sopair = { "3 city", "" };

	list.put(sopair);
	PRINTF_TEST("overwrite count",	list.getCount() == 1			);
	PRINTF_TEST("overwrite sizeof",	list.getBytes() == sopair.getBytes()	);

	// remove last element
	list.remove("3 city");

	// remove non existent from empty list
	list.remove("nonexistent");

	PRINTF_TEST("remove count",	list.getCount() == 0			);
	PRINTF_TEST("remove empty",	list.isEmpty()				);



	// TEST ITERATOR

	list_populate(list);

	iterator_test(module, list);
	iterator_get_test(module, list);

	// TEST REFERENCES

	list.removeAll();
	ref_test(module, list);

	// MOVE TESTS

	/* size = */ list_populate(list);

	LIST mlist = std::move(list);
	PRINTF_TEST("move c-tor 1",	mlist.getBytes() == size			);
	PRINTF_TEST("move c-tor 2",	list.isEmpty()				);
}

void list_test(const char *module, hm3::BlackHoleList &list){
	Pair p = nullptr;

	list_populate(list);

	PRINTF_TEST("print it",		true					);
	list.print();

	PRINTF_TEST("count",		list.getCount() == 0			);
	PRINTF_TEST("count estim",	list.getCount(true) == 0		);
	PRINTF_TEST("empty",		list.isEmpty()				);
	PRINTF_TEST("sizeof",		list.getBytes() == 0			);

	PRINTF_TEST("put",		list.put( { "key", "val" } )		);
	PRINTF_TEST("get",		! list.get("key")			);
	PRINTF_TEST("remove",		list.remove("key")			);
}

int main(int argc, char **argv){
	hm3::STLVectorList svl;
		list_test("STLVector", svl);

	// =========================

	hm3::VectorList vl;
		list_test("Vector", vl);

	// =========================

	hm3::LinkList ll;
		list_test("LinkList", ll);

	// =========================

	hm3::SkipList sl;
		list_test("SkipList", sl);
		if (0)
			skiplist_lanes_test(sl);

	// =========================
/*
	hm3::HashList<VectorList<> > hl_vl;
		list_test("HashList@Vector", hl_vl);

	// =========================

	hm3::HashList<LinkList> hl_ll;
		list_test("HashList@LinkList", hl_ll);

	// =========================
*/

	hm3::HashList<std::vector<hm3::SkipList> > hl_sl;
		list_test("HashList@SkipList", hl_sl);

	// =========================

	hm3::BlackHoleList bhl;
		list_test("BlackHoleList", bhl);


}


