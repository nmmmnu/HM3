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

	// ==================================

	int compare(const char *data, size_t const size) const;
	
	int compare(const char *data) const;
	int compare(const std::string &s) const;
	int compare(const StringRef &sr) const;

	// ==================================

	operator std::string() const;

	const char &operator [] (size_t index) const;

	// ==================================

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
	size_t		_size	= 0;
	const char	*_data	= "";

private:
	static size_t __strlen(const char *s);
	static const char *__strptr(const char *s);
	
	static int __compare(const char *s1, size_t const size1, const char *s2, size_t const size2);
};

std::ostream& operator << (std::ostream& os, const StringRef &sr);

// ==================================

inline StringRef::StringRef(const char *data, size_t const size) :
		_size(size),
		_data(__strptr(data)){}

inline StringRef::StringRef(const char *data) :
		StringRef(data, __strlen(data)){}

inline StringRef::StringRef(const std::string &s) :
		_size(s.size()),
		_data(s.data()){}

// ==================================

inline bool StringRef::empty() const{
	return _size == 0;
}

// ==================================

inline int StringRef::compare(const char *data, size_t const size) const{
	return __compare(_data, _size, data, size);
}

inline int StringRef::compare(const char *data) const{
	return compare(data, __strlen(data) );
}

inline int StringRef::compare(const std::string &s) const{
	return compare(s.data(), s.size() );
}

inline int StringRef::compare(const StringRef &sr) const{
	return compare(sr.data(), sr.size() );
}

// ==================================

inline StringRef::operator std::string() const{
	return std::string(_data, _size);
}

inline const char &StringRef::operator [] (size_t const index) const{
	return _data[index];
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

inline int StringRef::__compare(const char *s1, size_t const size1, const char *s2, size_t const size2){
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int res = memcmp(s1, s2, std::min(size1, size2 ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (size1 == size2)
		return 0;

	return size1 < size2 ? -1 : +1;
}

// ==================================

inline size_t StringRef::__strlen(const char *s){
	return s ? strlen(s) : 0;
}

inline const char *StringRef::__strptr(const char *s){
	return s ? s : "";
}

// ==================================

inline std::ostream& operator << (std::ostream& os, const StringRef &sr) {
	return os.write(sr.c_str(), sr.size());
}


#endif

