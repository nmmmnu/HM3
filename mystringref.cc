#include "mystringref.h"

#include <cstring>

// ==================================

std::ostream& operator << (std::ostream& os, const MyStringRef &sr) {
	for(size_t i = 0; i < sr.size(); ++i)
		os << sr[i];
	return os;
}

// ==================================

MyStringRef::MyStringRef(const char *data, size_t const size) :
		_size(size),
		_data(size ? data : __zeroStr){}

MyStringRef::MyStringRef(const char *data) :
		MyStringRef(data, data ? strlen(data) : 0){}

MyStringRef::MyStringRef(const std::string &s) :
		_size(s.size()),
		_data(s.data()){}

int MyStringRef::compare(const char *data) const{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	auto data_size = strlen(data);

	// check prefix
	if ( int res = memcmp(_data, data, std::min(_size, data_size ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (_size == data_size)
		return 0;

	return _size < data_size ? -1 : +1;
}
