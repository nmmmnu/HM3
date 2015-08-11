#ifndef _PAIR_H
#define _PAIR_H

#include "nmea0183checksumcalculator.h"

#include <memory>

#include <ostream>

#include "pairpod.h"

// ==============================

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

	static const bool CHECKSUM = true;

public:
	Pair() = default;

	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	Pair(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0);

	Pair(const void *blob /*, bool weak = false */ );

	operator bool() const;

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

public:
	static void setChecksumUsage(bool checksumUsage);

private:
	std::shared_ptr<const PairPOD> pod;

private:
	static uint64_t __getCreateTime(uint32_t created);

private:
	static PairPOD *_createBlob(size_t size);

	static PairPOD *_cloneBlob(const PairPOD *blob);

	static NMEA0183ChecksumCalculator __checksumCalculator;

	static bool __checksumUsage;

};

// ==============================

inline Pair::operator bool() const{
	return (bool) pod;
}

inline void Pair::setChecksumUsage(bool const checksumUsage){
	__checksumUsage = checksumUsage;
}

// ==============================

inline const char *Pair::getKey() const{
	return pod ? pod->getKey() : nullptr;
}

inline const char *Pair::getVal() const{
	return pod ? pod->getVal() : nullptr;
}

inline int Pair::cmp(const char *key) const{
	return pod ? pod->cmp(key) : 1;
}

inline int Pair::cmp(const Pair &pair) const{
	return cmp( pair.getKey() );
}

inline bool Pair::valid() const{
	return pod ? pod->valid() : false;
}

inline bool Pair::valid(const Pair &pair) const{
	return valid();
}

inline size_t Pair::getSize() const{
	return pod ? pod->getSize() : 0;
}

#endif
