#ifndef _PAIR_H
#define _PAIR_H

#include "std/std_optional.h"

#include "ichecksumcalculator.h"

#include <string.h>	//strcmp

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:	
	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	
	Pair(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0) : 
			Pair(key, value, value ? strlen(value) : 0, expires, created){};

	Pair(const void *blob, bool ownBlob = false) :
			blob((const Blob *) blob), 
			ownBlob(ownBlob){};

	Pair(Pair && other) :
			Pair(other.blob, other.ownBlob){
		other.ownBlob = false;
	};

	Pair(Pair & other) :
			Pair(other.cloneBlob(), true){};

	const Pair & operator=(const Pair& other);

	~Pair();

	inline operator bool() const;

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

	const void *cloneBlob() const;

	void print() const;
	
public:
	static inline void setChecksumCalculator(IChecksumCalculator & checksumCalculator);
	static inline void removeChecksumCalculator();

private:
	struct Blob;
	const Blob *blob = nullptr;
	bool ownBlob = true;

private:
	static uint64_t __getCreateTime(uint32_t created);
	static uint8_t __getChecksum(const void *buffer, size_t size);
	constexpr static size_t __sizeofBase();

	size_t _sizeofBuffer() const;
	uint8_t _getChecksum() const;

private:
	static std_optional<IChecksumCalculator> __checksumCalculator;

};

// ==============================

inline Pair::operator bool() const{
	return blob;
}

inline int Pair::cmp(const char *key) const{
	if (blob == nullptr)
		return 1;

	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline int Pair::cmp2(const Pair &pair) const{
	return cmp(pair.getKey());
}

inline void Pair::setChecksumCalculator(IChecksumCalculator & checksumCalculator){
	__checksumCalculator = &checksumCalculator;
}

inline void Pair::removeChecksumCalculator(){
	__checksumCalculator = nullptr;
}

#endif
