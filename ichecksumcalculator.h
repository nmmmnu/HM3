#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator(){};

	virtual uint8_t calcChecksum(const void *data2, size_t len) = 0;
};

#endif
