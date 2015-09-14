#include "stringref.h"

#include <cstring>

// ==================================

StringRef::StringRef(const char *data, size_t const size) :
		_size(size),
		_data(_strptr(data)){}

StringRef::StringRef(const char *data) :
		StringRef(data, _strlen(data)){}

StringRef::StringRef(const std::string &s) :
		_size(s.size()),
		_data(s.data()){}

// ==================================

int StringRef::compare(const char *data, size_t const size) const{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int res = memcmp(_data, data, std::min(_size, size ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (_size == size)
		return 0;

	return _size < size ? -1 : +1;
}

// ==================================

size_t StringRef::_strlen(const char *s){
	return s ? strlen(s) : 0;
}

const char *StringRef::_strptr(const char *s){
	return s ? s : "";
}


