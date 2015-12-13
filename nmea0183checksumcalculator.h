#ifndef _NMEA0183_CHECKSUM_CALCULATOR_H
#define _NMEA0183_CHECKSUM_CALCULATOR_H

#include <cstdlib>
#include <cstdint>

struct NMEA0183ChecksumCalculator{
	using result_type = uint8_t;

	result_type operator()(const void *data2, size_t len, result_type start = 0) const noexcept;
};

#include "checksum.h"

#endif

