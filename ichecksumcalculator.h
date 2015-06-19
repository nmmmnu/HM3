#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>

class IChecksumCalculator{
public:
	virtual uint8_t calcChecksum(const char *data2, size_t len) = 0;
};

#endif
