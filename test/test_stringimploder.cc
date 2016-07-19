#include "stringimploder.h"

#include <vector>

#include <cassert>

static int test_stringimploder(){
	StringImploder imploder;

	using container = std::vector<std::string>;

	assert(imploder(container{ "hello", "world" },	' ') == "hello world"	);
	assert(imploder(container{ "how", "are", "u" },	' ') == "how are u"	);
	assert(imploder(container{ "I", "am", "OK" },	'.') == "I.am.OK"	);

	return 0;
}

int main(){
	return test_stringimploder();
}

