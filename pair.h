#ifndef _PAIR_H
#define _PAIR_H

#include "nmea0183checksumcalculator.h"

#include <string.h>	//strcmp

#include <memory>
#include <ostream>

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

	static const bool CHECKSUM = true;

public:
	Pair() = default;

	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	Pair(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0);

	Pair(const void *blob);
	Pair(const void *blob, bool weak);

	operator bool() const;

public:
	const char *getKey() const;
	const char *getVal() const;

	int cmp(const char *key) const;
	int cmp(const Pair &pair) const;

	bool valid() const;
	bool valid(const Pair &pair) const;

	bool fwrite(std::ostream & os) const;

	size_t getSize() const;

	void print() const;

public:
	static void setChecksumUsage(bool checksumUsage);

private:
	struct Blob;

	std::shared_ptr<Blob>	_blob;

private:
	static uint64_t __getCreateTime(uint32_t created);
	constexpr
	static size_t __sizeofBase();

	size_t  _sizeofBuffer() const;

private:
	static NMEA0183ChecksumCalculator __checksumCalculator;

	static bool __checksumUsage;

};

// ==============================

inline Pair::operator bool() const{
	return (bool) _blob;
}

inline int Pair::cmp(const char *key) const{
	if (! _blob)
		return 1;

	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline int Pair::cmp(const Pair &pair) const{
	return cmp(pair.getKey());
}

inline bool Pair::valid(const Pair &pair) const{
	return valid();
}

inline void Pair::setChecksumUsage(bool checksumUsage){
	__checksumUsage = checksumUsage;
}

#endif
