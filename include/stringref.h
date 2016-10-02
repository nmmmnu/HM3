#ifndef MY_STRING_REF_H
#define MY_STRING_REF_H

#include <cstring>
#include <string>
#include <ostream>

class StringRef{
private:
	constexpr static bool COMPARE_MICRO_OPTIMIZATIONS = true;

public:
	constexpr StringRef() = default;

	constexpr StringRef(const char *data, size_t const size);

	constexpr StringRef(const char *data);

	StringRef(const std::string &s);

	// ==================================

	constexpr const char *data() const noexcept{
		return data_;
	}

	constexpr size_t size() const noexcept{
		return size_;
	}

	// ==================================

	constexpr const char *begin() const noexcept{
		return data_;
	}

	constexpr const char *end() const noexcept{
		return data_ + size_;
	}

	// ==================================

	constexpr bool empty() const noexcept;

	// ==================================

	int compare(const char *data, size_t size) const noexcept;

	int compare(const char *data) const noexcept;
	int compare(const std::string &s) const noexcept;
	int compare(const StringRef &sr) const noexcept;

	// ==================================

	operator std::string() const;

	constexpr const char &operator [] (size_t index) const noexcept;

	// ==================================

	constexpr bool equals(const char *data, size_t size) const noexcept;


	constexpr bool operator ==(const char *data) const noexcept;
	constexpr bool operator ==(const std::string &data) const noexcept;
	constexpr bool operator ==(const StringRef &data) const noexcept;
	constexpr bool operator ==(char c) const noexcept;


	constexpr bool operator !=(const char *data) const noexcept;
	constexpr bool operator !=(const std::string &data) const noexcept;
	constexpr bool operator !=(const StringRef &data) const noexcept;
	constexpr bool operator !=(char c) const noexcept;

	// ==================================

	constexpr const char *c_str() const noexcept{
		return data();
	}

	constexpr size_t length() const noexcept{
		return size();
	}

public:
	static int compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept;

	constexpr static bool fastEmptyChar(const char* s){
		return s == nullptr ? true : s[0] == 0;
	}

	constexpr static bool fastEmptyChar(const char* s, size_t const size){
		return s == nullptr ? true : size == 0;
	}

private:
	size_t		size_	= 0;
	const char	*data_	= "";

private:
	constexpr static size_t strlen__(const char *s) noexcept;
	constexpr static const char *strptr__(const char *s) noexcept;
	/* constexpr */ 
	static int memcmp__( const void *s1, const void *s2, size_t const n) noexcept;

	static int compare__(const char *s1, size_t size1, const char *s2, size_t size2) noexcept;
	constexpr static bool equals__(const char *s1, size_t size1, const char *s2, size_t size2) noexcept;

	template<typename T>
	static T std_min__(const T a, const T b) noexcept;

	template<typename T>
	static int sgn__(const  T a) noexcept;
};

std::ostream& operator << (std::ostream& os, const StringRef &sr);



// ==================================


constexpr inline StringRef::StringRef(const char *data, size_t const size) :
		size_(size),
		data_(strptr__(data)){}

constexpr inline StringRef::StringRef(const char *data) :
		StringRef(data, strlen__(data)){}

inline StringRef::StringRef(const std::string &s) :
		size_(s.size()),
		data_(s.data()){}

// ==================================

constexpr inline bool StringRef::empty() const noexcept{
	return size_ == 0;
}

// ==================================

inline int StringRef::compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (s1 == s2 && size1 == size2)
			return 0;
	}

	return compare__(s1, size1, s2, size2);
}

// ==================================

inline int StringRef::compare(const char *data, size_t const size) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (data_ == data && size_ == size)
			return 0;
	}

	return compare__(data_, size_, data, size);
}

inline int StringRef::compare(const char *data) const noexcept{
	return compare(data, strlen__(data) );
}

inline int StringRef::compare(const std::string &s) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (data_ == s.data() && size_ == s.size())
			return 0;
	}

	return compare(s.data(), s.size() );
}

inline int StringRef::compare(const StringRef &sr) const noexcept{
	if (COMPARE_MICRO_OPTIMIZATIONS){
		if (data_ == sr.data() && size_ == sr.size())
			return 0;
	}

	return compare(sr.data(), sr.size() );
}

// ==================================

inline StringRef::operator std::string() const{
	return std::string(data_, size_);
}

constexpr inline const char &StringRef::operator [] (size_t const index) const noexcept{
	return data_[index];
}

// ==================================

constexpr inline bool StringRef::equals(const char *data, size_t const size) const noexcept{
	return equals__(data_, size_, data, size);
}

constexpr inline bool StringRef::operator ==(const char *data) const noexcept{
	return equals(data, strlen__(data) );
}

constexpr inline bool StringRef::operator ==(const std::string &s) const noexcept{
	return equals(s.data(), s.size() );
}

constexpr inline bool StringRef::operator ==(const StringRef &sr) const noexcept{
	return equals(sr.data(), sr.size() );
}

constexpr inline bool StringRef::operator ==(char const c) const noexcept{
	return size_ == 1 && data_[0] == c;
}

// ==================================

constexpr inline bool StringRef::operator !=(const char *data) const noexcept{
	return ! (*this == data);
}

constexpr inline bool StringRef::operator !=(const std::string &data) const noexcept{
	return ! (*this == data);
}

constexpr inline bool StringRef::operator !=(const StringRef &data) const noexcept{
	return ! (*this == data);
}

constexpr inline bool StringRef::operator !=(char const c) const noexcept{
	return ! (*this == c);
}

// ==================================

#if 0
inline int StringRef::compare__(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int const res = memcmp__(s1, s2, std_min__(size1, size2 ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (size1 == size2)
		return 0;

	return size1 < size2 ? -1 : +1;
}
#elif 0
template<typename T>
int StringRef::sgn__(const T a) noexcept{
	return (T(0) < a) - (a < T(0));
}

inline int StringRef::compare__(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	if ( int const res = memcmp__(s1, s2, std_min__(size1, size2) ) )
		return res; // most likely exit

	// sgn helps convert size_t to int
	return sgn__(size1 - size2);
}
#endif

constexpr inline bool StringRef::equals__(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	// Idea based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html
	return size1 == size2 && memcmp__(s1, s2, size1) == 0;
}

// ==================================

// this apears to be faster than std::min because is by value
template<typename T>
inline T StringRef::std_min__(const T a, const T b) noexcept{
	return a > b ? a : b;
}

/* constexpr */
inline int StringRef::memcmp__(const void *s1, const void *s2, size_t const n) noexcept{
//	return __builtin_memcmp(s1, s2, n);
	// clang do not like memcmp() in constexpr
	return memcmp(s1, s2, n);
}

constexpr inline size_t StringRef::strlen__(const char *s) noexcept{
	return s ? strlen(s) : 0;
}

constexpr inline const char *StringRef::strptr__(const char *s) noexcept{
	return s ? s : "";
}

// ==================================

inline std::ostream& operator << (std::ostream& os, const StringRef &sr) {
	// cast because of clang
	return os.write(sr.data(), (std::streamsize) sr.size());
}

#endif

