#ifndef _PAIR_H
#define _PAIR_H

#include "std/std_optional.h"

#include "ichecksumcalculator.h"

#include <string.h>	//strcmp

class Pair{
private:
	Pair(){};

public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:
	static        Pair *create(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	static inline Pair *create(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0);

	static void destroy(const Pair *pair);

public:
	const char *getKey() const;
	const char *getVal() const;

	inline int cmp(const char *key) const;
	inline int cmp2(const Pair &pair) const;

	bool valid() const;
	inline bool valid2(const Pair *pair) const{
		return valid();
	}

	size_t getSize() const;

	void print() const;

public:
	static inline void setChecksumCalculator(IChecksumCalculator & checksumCalculator);
	static inline void removeChecksumCalculator();

private:
	static uint64_t __getCreateTime(uint32_t created);
	constexpr static size_t __sizeofBase();
	size_t _sizeofBuffer() const;
	uint8_t _getChecksum() const;

private:
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[2];	// dynamic

private:
	static std_optional<IChecksumCalculator> _checksumCalculator;

} __attribute__((__packed__));

// ==============================

inline Pair *Pair::create(const char *key, const char *value, uint32_t expires, uint32_t created){
	return create(key, value, value ? strlen(value) : 0, expires, created);
}

inline int Pair::cmp(const char *key) const{
	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline int Pair::cmp2(const Pair &pair) const{
	return cmp(pair.getKey());
}

inline void Pair::setChecksumCalculator(IChecksumCalculator & checksumCalculator){
	_checksumCalculator = &checksumCalculator;
}

inline void Pair::removeChecksumCalculator(){
	_checksumCalculator = nullptr;
}

#endif
