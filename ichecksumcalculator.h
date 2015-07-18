#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <string.h>	// strlen

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator(){};

	uint8_t calcChecksum(const void *data, size_t size);
	uint8_t calcChecksum(const char *data);
	
private:
	virtual uint8_t _calcChecksum(const void *data, size_t size) = 0;
};


inline uint8_t IChecksumCalculator::calcChecksum(const void *data, size_t size){
	return data ? _calcChecksum(data, size) : 0;
}

inline uint8_t IChecksumCalculator::calcChecksum(const char *data){
	return calcChecksum(data, strlen(data));
}

#endif
