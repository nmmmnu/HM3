#ifndef _PAIR_H
#define _PAIR_H

#include "std/std_optional.h"
#include "ichecksumcalculator.h"

#include <string.h>	//strcmp
#include <stdio.h>	//FILE

class Pair{
private:
	Pair(){};

public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

private:
	static std_optional<IChecksumCalculator> _checksumCalculator;

public:
	static Pair *create(const char *key, const void *value, size_t valLen, uint32_t expires = 0);
	static inline Pair *create(const char *key, const char *value, uint32_t expires = 0);
	static inline Pair *create(const char *key, uint32_t expires = 0);

	static void destroy(Pair *pair);

	static inline void setChecksumCalculator(IChecksumCalculator & checksumCalculator);
	static inline void removeChecksumCalculator();

public:
	const char *getKey() const;
	const char *getVal() const;

	inline int cmp(const char *key) const;
	inline int cmp(const Pair *pair) const;

	bool valid() const;
	bool valid(const Pair &pair) const;
	bool valid(const Pair *pair) const;

	size_t getSize() const;

	bool writeToFile(FILE *F) const;

	void print() const;

private:
	static constexpr size_t __sizeofPair();
	size_t _sizeofBuffer() const;

	uint8_t _getChecksum() const;

private:
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[2];	// dynamic, at least 2
} __attribute__((__packed__));

// ==============================

inline Pair *Pair::create(const char *key, const char *value, uint32_t expires){
	return create(key, value, value ? strlen(value) : 0, expires);
}

inline Pair *Pair::create(const char *key, uint32_t expires){
	return create(key, nullptr, 0, expires);
}

inline void Pair::setChecksumCalculator(IChecksumCalculator & checksumCalculator){
	_checksumCalculator = &checksumCalculator;
}

inline void Pair::removeChecksumCalculator(){
	_checksumCalculator = NULL;
}

inline int Pair::cmp(const char *key) const{
	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline int Pair::cmp(const Pair *pair) const{
	return pair == nullptr ? -1 : cmp(pair->getKey());
}

// ==============================


#endif
