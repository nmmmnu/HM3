#ifndef _PAIR_H
#define _PAIR_H

#include "std/std_optional.h"

#include "ichecksumcalculator.h"

#include <string.h>	//strcmp
//#include <stdio.h>

class Pair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:	
	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0, uint32_t created = 0);
	
	Pair(const char *key, const char *value,                uint32_t expires = 0, uint32_t created = 0) : 
			Pair(key, value, value ? strlen(value) : 0, expires, created){};

	Pair(const void *blob, bool ownBlob = false) :
			_blob((const Blob *) blob), 
			_ownBlob(ownBlob){};

	Pair(Pair && other);
	Pair(const Pair & other);
	Pair & operator=(Pair other);

	~Pair();

	inline operator bool() const;

public:
	const char *getKey() const;
	const char *getVal() const;

	inline int cmp(const char *key) const;
	inline int cmp(const Pair &pair) const;

	bool valid() const;
	bool valid(const Pair &pair) const{
		return valid();
	}

	size_t getSize() const;

	const void *cloneBlob() const;
	inline const void *getBlob() const;

	inline void getBlobOwnership();

	void print() const;
	
public:
	static inline void setChecksumCalculator(IChecksumCalculator & checksumCalculator);
	static inline void removeChecksumCalculator();

private:
	struct Blob;
	const Blob *_blob = nullptr;
	bool _ownBlob = true;

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
	return _blob;
}

inline const void *Pair::getBlob() const{
	return _blob;
}

inline int Pair::cmp(const char *key) const{
	if (_blob == nullptr)
		return 1;

	return key == nullptr ? -1 : strcmp(getKey(), key);
}

inline int Pair::cmp(const Pair &pair) const{
	return cmp(pair.getKey());
}

inline void Pair::getBlobOwnership(){
	_ownBlob = true;
}


inline void Pair::setChecksumCalculator(IChecksumCalculator & checksumCalculator){
	__checksumCalculator = &checksumCalculator;
}

inline void Pair::removeChecksumCalculator(){
	__checksumCalculator = nullptr;
}

#endif
