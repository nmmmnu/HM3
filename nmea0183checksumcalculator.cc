#include "nmea0183checksumcalculator.h"

#include <stdint.h>

auto NMEA0183ChecksumCalculator::operator()(const void *data2, size_t const size, uint8_t const start) const noexcept -> result_type{
	// NMEA 0183 like checksum
	const uint8_t *data = (const uint8_t *) data2;
	uint8_t sum = start;

	size_t i;
	for(i = 0; i < size; ++i)
		sum = sum ^ data[i];

	return sum;
}

//template class Checksum<NMEA0183ChecksumCalculator>;
