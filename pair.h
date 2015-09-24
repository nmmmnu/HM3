#ifndef _PAIR_H
#define _PAIR_H

#include <string>
#include <ostream>
#include <utility>	// swap

#include "stringref.h"

#include "mytime.h"
#include "stringref.h"

// ==============================

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:
	Pair() = default;

	Pair(const StringRef &key, const StringRef &val, uint32_t expires = 0, uint32_t created = 0);

	static Pair tombstone(const StringRef &key){
		return Pair(key, StringRef{} );
	}

	Pair(const void *blob);

	operator bool() const noexcept;
//	operator const void *() const;

public:
	const std::string &getKey() const noexcept;
	const std::string &getVal() const noexcept;
	int cmp(const StringRef &key) const noexcept;
	int cmp(const char *key) const noexcept;
	int cmp(const Pair &pair) const noexcept;
	bool isTombstone() const noexcept;
	bool valid() const noexcept;
	bool valid(const Pair &pair) const noexcept;
	size_t getSize() const noexcept;

public:
	bool fwrite(std::ostream & os) const;

	void print() const noexcept;

	void swap(Pair &other) noexcept;

private:
	uint64_t	created;
	uint32_t	expires;
	std::string	key;
	std::string	val;

private:
	static uint64_t __getCreateTime(uint32_t created) noexcept;

};

inline void swap(Pair &a, Pair &b) noexcept{
	a.swap(b);
}

// ==============================

inline Pair::operator bool() const noexcept{
	return valid();
}

// ==============================

inline const std::string &Pair::getKey() const noexcept{
	return key;
}

inline const std::string &Pair::getVal() const noexcept{
	return val;
}

inline bool Pair::isTombstone() const noexcept{
	return val.empty();
}

inline int Pair::cmp(const StringRef &key2) const noexcept{
	if (key2.empty())
		return -1;

	// std::string.compare gives 0 if internal string is nullptr
	if (key.empty())
		return +1;

	//return key.compare(key2);
	return - key2.compare(key);
}

inline int Pair::cmp(const char *key) const noexcept{
	return cmp( StringRef{key} );
}

inline int Pair::cmp(const Pair &pair) const noexcept{
	return cmp( pair.getKey() );
}

inline bool Pair::valid() const noexcept{
	// check key size
	if (key.empty())
		return false;

	// check expires
	if (expires && MyTime::expired(created, expires) )
		return false;

	// finally all OK
	return true;
}

inline bool Pair::valid(const Pair &pair) const noexcept{
	return valid();
}

#endif
