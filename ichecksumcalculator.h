#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <string.h>	// strlen

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator(){};

	uint8_t calc(const void *data, size_t size);
	uint8_t calc(const char *data);

private:
	virtual uint8_t _calc(const void *data, size_t size) = 0;
};


inline uint8_t IChecksumCalculator::calc(const void *data, size_t size){
	return _calc(data, size);
}

inline uint8_t IChecksumCalculator::calc(const char *data){
	return calc(data, strlen(data));
}

#endif
