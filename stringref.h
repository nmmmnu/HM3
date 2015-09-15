#ifndef MY_STRING_REF_H
#define MY_STRING_REF_H

#include <cstring>
#include <string>
#include <ostream>

class StringRef{
public:
	StringRef() = default;

	StringRef(const char *data, size_t const size);
	StringRef(const char *data);
	StringRef(const std::string &s);

	// ==================================

	const char *data() const{
		return _data;
	}

	size_t size() const{
		return _size;
	}

	// ==================================

	bool empty() const;

	char charAt(size_t index) const;

	std::string toString() const;

	// ==================================

	int compare(const char *data) const;
	int compare(const std::string &s) const;
	int compare(const StringRef &sr) const;

	// ==================================

	operator std::string() const;

	char operator [] (size_t index) const;

	bool operator ==(const char *data) const;
	bool operator ==(const std::string &data) const;
	bool operator ==(const StringRef &data) const;
	bool operator ==(char c) const;

	bool operator !=(const char *data) const;
	bool operator !=(const std::string &data) const;
	bool operator !=(const StringRef &data) const;
	bool operator !=(char c) const;

	// ==================================

	const char *c_str() const{
		return data();
	}

	size_t length() const{
		return size();
	}

private:
	size_t			_size	= 0;
	const char		*_data	= __zeroStr;

private:
	constexpr
	static const char	*__zeroStr = "";
};

std::ostream& operator << (std::ostream& os, const StringRef &sr);

// ==================================

inline StringRef::StringRef(const char *data, size_t const size) :
		_size(size),
		_data(size ? data : __zeroStr){}

inline StringRef::StringRef(const char *data) :
		StringRef(data, data ? strlen(data) : 0){}

inline StringRef::StringRef(const std::string &s) :
		_size(s.size()),
		_data(s.data()){}

// ==================================

inline bool StringRef::empty() const{
	return _size == 0;
}

inline char StringRef::charAt(size_t const index) const{
	return _data[index];
}

inline std::string StringRef::toString() const{
	return std::string(_data, _size);
}

// ==================================

inline int StringRef::compare(const std::string &s) const{
	return compare(s.data() );
}

inline int StringRef::compare(const StringRef &sr) const{
	return compare(sr.data() );
}

// ==================================

inline StringRef::operator std::string() const{
	return toString();
}

inline char StringRef::operator [] (size_t const index) const{
	return charAt(index);
}

// ==================================

inline bool StringRef::operator ==(const char *data) const{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(const std::string &data) const{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(const StringRef &data) const{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(char const c) const{
	return _size == 1 && _data[0] == c;
}

// ==================================

inline bool StringRef::operator !=(const char *data) const{
	return ! (*this == data);
}

inline bool StringRef::operator !=(const std::string &data) const{
	return ! (*this == data);
}

inline bool StringRef::operator !=(const StringRef &data) const{
	return ! (*this == data);
}

inline bool StringRef::operator !=(char const c) const{
	return ! (*this == c);
}

// ==================================

inline int StringRef::compare(const char *data) const{
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

// ==================================

inline std::ostream& operator << (std::ostream& os, const StringRef &sr) {
	for(size_t i = 0; i < sr.size(); ++i)
		os << sr[i];
	return os;
}

#endif

