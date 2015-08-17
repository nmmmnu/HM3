#ifndef _PAIR_H
#define _PAIR_H

#include <string>
#include <ostream>

#include "mytime.h"

// ==============================

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:
	Pair() = default;

//	Pair(std::string &key, std::string &value,              uint32_t expires = 0, uint32_t created = 0);

	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	Pair(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0);

	Pair(const void *blob);

	operator bool() const;
	operator const void *() const;

public:
	const char *getKey() const;
	const char *getVal() const;
	int cmp(const char *key) const;
	int cmp(const Pair &pair) const;
	bool valid() const;
	bool valid(const Pair &pair) const;
	size_t getSize() const;

public:
	bool fwrite(std::ostream & os) const;

	void print() const;

private:
	uint64_t	created;
	uint32_t	expires;
	std::string	key;
	std::string	val;

private:
	static uint64_t __getCreateTime(uint32_t created);

};

// ==============================

inline Pair::operator bool() const{
	return valid();
}

// ==============================

inline const char *Pair::getKey() const{
	return key.empty() ? nullptr : key.c_str();
}

inline const char *Pair::getVal() const{
	return val.empty() ? nullptr : val.c_str();
}

inline int Pair::cmp(const char *key2) const{
	if (key2 == nullptr)
		return -1;

	// std::string.compare gives 0 if internal string is nullptr
	if (getKey() == nullptr)
		return +1;

	return key.compare(key2);
}

inline int Pair::cmp(const Pair &pair) const{
	return cmp( pair.getKey() );
}

inline bool Pair::valid() const{
	// check key size
	if (key.empty())
		return false;

	// check expires
	if (expires && MyTime::expired(created, expires) )
		return false;

	// finally all OK
	return true;
}

inline bool Pair::valid(const Pair &pair) const{
	return valid();
}

#endif
