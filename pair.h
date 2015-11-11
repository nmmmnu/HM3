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

	operator bool() const noexcept{
		return valid();
	}

public:
	const std::string &getKey() const noexcept{
		return key;
	}

	const std::string &getVal() const noexcept{
		return val;
	}

	int cmp(const StringRef &key) const noexcept;

	int cmp(const char *key) const noexcept{
		return cmp( StringRef{key} );
	}

	int cmp(const Pair &pair) const noexcept{
		return cmp( pair.getKey() );
	}

	bool isTombstone() const noexcept{
		return val.empty();
	}

	bool valid(bool tombstoneCheck = false) const noexcept;

	bool valid(const Pair &pair, bool tombstoneCheck = false) const noexcept{
		return valid(tombstoneCheck);
	}

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

inline int Pair::cmp(const StringRef &key2) const noexcept{
	if (key2.empty())
		return -1;

	// std::string.compare gives 0 if internal string is nullptr
	if (key.empty())
		return +1;

	//return key.compare(key2);
	return - key2.compare(key);
}

inline bool Pair::valid(bool tombstoneCheck) const noexcept{
	// check key size
	if (key.empty())
		return false;

	// check val size
	if (tombstoneCheck && val.empty())
		return false;

	// check expires
	if (expires && MyTime::expired(created, expires) )
		return false;

	// finally all OK
	return true;
}

#endif
