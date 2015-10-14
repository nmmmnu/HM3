#ifndef _NMEA0183_CHECKSUM_CALCULATOR_H
#define _NMEA0183_CHECKSUM_CALCULATOR_H

#include "checksum.h"

struct NMEA0183ChecksumCalculator{
	typedef uint8_t result_type;

	result_type operator()(const void *data2, size_t len, uint8_t start = 0) const noexcept;
};

#endif

