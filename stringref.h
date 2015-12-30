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
	static int compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
		return __compare(s1, size1, s2, size2);
	}

private:
	size_t		_size	= 0;
	const char	*_data	= "";

private:
	static size_t __strlen(const char *s) noexcept;
	static const char *__strptr(const char *s) noexcept;

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

inline int StringRef::compare(const char *data, size_t const size) const noexcept{
	return __compare(_data, _size, data, size);
}

inline int StringRef::compare(const char *data) const noexcept{
	return compare(data, __strlen(data) );
}

inline int StringRef::compare(const std::string &s) const noexcept{
	return compare(s.data(), s.size() );
}

inline int StringRef::compare(const StringRef &sr) const noexcept{
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

inline int StringRef::__compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int res = memcmp(s1, s2, __std_min(size1, size2 ) ) )
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
	return os.write(sr.data(), sr.size());
}

#endif

