#ifndef _PAIR_BLOB_H
#define _PAIR_BLOB_H

#include <string.h>	// strcmp
#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include "mytime.h"

#include "nmea0183checksumcalculator.h"

class Pair;

struct PairPOD{

public:
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[1];	// dynamic

public:
	const char *getKey() const;
	const char *getVal() const;

	int cmp(const char *key) const;

	bool valid() const;
//	bool valid(const PairPOD &pair) const;

	uint8_t calcChecksum() const;

	size_t getSize() const;

private:
	size_t  _sizeofBuffer() const;

	friend class Pair;
	constexpr
	static size_t __sizeofBase();

} __attribute__((__packed__));

// ==============================

inline const char *PairPOD::getKey() const{
	return buffer;
}

inline const char *PairPOD::getVal() const{
	// vallen is 0 no matter of endianness
	if (vallen == 0)
		return nullptr;

	return & buffer[ be16toh(keylen) + 1 ];
}

inline int PairPOD::cmp(const char *key) const{
	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline bool PairPOD::valid() const{
	// check key size
	if (keylen == 0)
		return false;

	// now expires is 0 no matter of endianness
	if (expires && MyTime::expired( be64toh(created), be32toh(expires) ) )
		return false;

	// calc checksum
	if (calcChecksum() != checksum)
		return false;

	// finally all OK
	return true;
}

inline size_t PairPOD::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

inline size_t PairPOD::_sizeofBuffer() const{
	return be16toh(keylen) + 1 + be32toh(vallen) + 1;
}

constexpr
inline size_t PairPOD::__sizeofBase(){
	return offsetof(PairPOD, buffer);
}

inline uint8_t PairPOD::calcChecksum() const{
	NMEA0183ChecksumCalculator chk;

	return chk.calc(buffer, _sizeofBuffer());
}

#endif

