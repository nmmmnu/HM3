#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator(){};

	virtual uint8_t calcChecksum(const void *data, size_t size) = 0;
};

#endif
