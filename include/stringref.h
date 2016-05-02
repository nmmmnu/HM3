#ifndef MY_STRING_REF_H
#define MY_STRING_REF_H

#include <cstring>
#include <string>
#include <ostream>

class StringRef{
private:
	constexpr static bool COMPARE_MICRO_OPTIMIZATIONS = true;

public:
	StringRef() = default;

	StringRef(const char *data, size_t const size);
	StringRef(const char *data);
	StringRef(const std::string &s);

	// ==================================

	const char *data() const noexcept{
		return _data;
	}

	size_t size() const noexcept{
		return _size;
	}

	// ==================================

	bool empty() const noexcept;

	// ==================================

	int compare(const char *data, size_t const size) const noexcept;

	int compare(const char *data) const noexcept;
	int compare(const std::string &s) const noexcept;
	int compare(const StringRef &sr) const noexcept;

	// ==================================

	operator std::string() const;

	const char &operator [] (size_t index) const noexcept;

	// ==================================

	bool operator ==(const char *data) const noexcept;
	bool operator ==(const std::string &data) const noexcept;
	bool operator ==(const StringRef &data) const noexcept;
	bool operator ==(char c) const noexcept;

	bool operator !=(const char *data) const noexcept;
	bool operator !=(const std::string &data) const noexcept;
	bool operator !=(const StringRef &data) const noexcept;
	bool operator !=(char c) const noexcept;

	// ==================================

	const char *c_str() const noexcept{
		return data();
	}

	size_t length() const noexcept{
		return size();
	}

public:
	static int compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept;

	static bool fastEmptyChar(const char* s){
		return s == nullptr ? true : s[0] == 0;
	}

	static bool fastEmptyChar(const char* s, size_t const size){
		return s == nullptr ? true : size == 0;
	}

private:
	size_t		_size	= 0;
	const char	*_data	= "";

private:
	static size_t __strlen(const char *s) noexcept;
	static const char *__strptr(const char *s) noexcept;

	static int __memcmp( const void *s1, const void *s2, size_t const n) noexcept;
	static int __compare(const char *s1, size_t size1, const char *s2, size_t size2) noexcept;

	static size_t __std_min(size_t const a, size_t const b) noexcept;
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

inline bool StringRef::empty() const noexcept{
	return _size == 0;
}

// ==================================

inline int StringRef::compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (s1 == s2 && size1 == size2)
			return 0;
	}

	return __compare(s1, size1, s2, size2);
}

// ==================================

inline int StringRef::compare(const char *data, size_t const size) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (_data == data && _size == size)
			return 0;
	}

	return __compare(_data, _size, data, size);
}

inline int StringRef::compare(const char *data) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (_data == data){
			size_t const size = __strlen(data);
			if (_size == size)
				return 0;

			return compare(data, size);
		}
	}

	return compare(data, __strlen(data) );
}

inline int StringRef::compare(const std::string &s) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (_data == s.data() && _size == s.size())
			return 0;
	}

	return compare(s.data(), s.size() );
}

inline int StringRef::compare(const StringRef &sr) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (_data == sr.data() && _size == sr.size())
			return 0;
	}

	return compare(sr.data(), sr.size() );
}

// ==================================

inline StringRef::operator std::string() const{
	return std::string(_data, _size);
}

inline const char &StringRef::operator [] (size_t const index) const noexcept{
	return _data[index];
}

// ==================================

inline bool StringRef::operator ==(const char *data) const noexcept{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(const std::string &data) const noexcept{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(const StringRef &data) const noexcept{
	return compare(data) == 0;
}

inline bool StringRef::operator ==(char const c) const noexcept{
	return _size == 1 && _data[0] == c;
}

// ==================================

inline bool StringRef::operator !=(const char *data) const noexcept{
	return ! (*this == data);
}

inline bool StringRef::operator !=(const std::string &data) const noexcept{
	return ! (*this == data);
}

inline bool StringRef::operator !=(const StringRef &data) const noexcept{
	return ! (*this == data);
}

inline bool StringRef::operator !=(char const c) const noexcept{
	return ! (*this == c);
}

// ==================================

inline int StringRef::__memcmp(const void *s1, const void *s2, size_t const n) noexcept{
//	return __builtin_memcmp(s1, s2, n);
	return memcmp(s1, s2, n);
}

inline int StringRef::__compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int res = __memcmp(s1, s2, __std_min(size1, size2 ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (size1 == size2)
		return 0;

	return size1 < size2 ? -1 : +1;
}

// ==================================

inline size_t StringRef::__strlen(const char *s) noexcept{
	return s ? strlen(s) : 0;
}

inline const char *StringRef::__strptr(const char *s) noexcept{
	return s ? s : "";
}

// this apears to be faster than std::min
inline size_t StringRef::__std_min(size_t const a, size_t const b) noexcept{
	return a > b ? a : b;
}

// ==================================

inline std::ostream& operator << (std::ostream& os, const StringRef &sr) {
	// cast because of clang
	return os.write(sr.data(), (std::streamsize) sr.size());
}

#endif

