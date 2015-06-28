#ifndef _PAIR_H
#define _PAIR_H

#include "std/std_optional.h"
#include "ichecksumcalculator.h"
#include "ipair.h"

#include <string.h>	//strcmp

struct PairBlob;

class Pair : virtual public IPair{
public:
	Pair(const void *blob) : IPair(blob){};

	Pair(Pair && other) : IPair(&other){};

	Pair(const char *key, const void *value, size_t valLen, uint32_t expires = 0) :
		IPair( __createBuffer(key, value, valLen, expires), true ){};

	Pair(const char *key, const char *value, uint32_t expires = 0) :
		Pair(key, value, value ? strlen(value) : 0, expires){};

	inline Pair & operator=(const void *blob){
		setBlob(blob);
		return *this;
	}

	inline Pair & operator=(Pair pair){
		return operator=(pair.getBlob());
	};

public:
	virtual const char *getKey() const override;
	virtual const char *getVal() const override;

	virtual int cmp(const char *key) const override;
	virtual bool valid() const override;

	virtual size_t getSize() const override;

	virtual void print() const override;

public:
	static inline void setChecksumCalculator(IChecksumCalculator & checksumCalculator);
	static inline void removeChecksumCalculator();

private:
	inline PairBlob *_getBlob() const;

	static const void *__createBuffer(const char *key, const void *value, size_t valLen, uint32_t expires);
	static constexpr size_t __sizeofBase();
	size_t _sizeofBuffer() const;

	static uint8_t __getChecksum(const void *buffer, size_t size);

private:
	static std_optional<IChecksumCalculator> _checksumCalculator;
};

// ==============================

inline PairBlob *Pair::_getBlob() const{
	return (PairBlob *) getBlob();
}

inline void Pair::setChecksumCalculator(IChecksumCalculator & checksumCalculator){
	_checksumCalculator = &checksumCalculator;
}

inline void Pair::removeChecksumCalculator(){
	_checksumCalculator = NULL;
}

#endif
