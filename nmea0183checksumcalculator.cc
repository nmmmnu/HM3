#include "nmea0183checksumcalculator.h"

uint8_t NMEA0183ChecksumCalculator::calcChecksum(const void *data2, size_t len){
	// NMEA 0183 like checksum
	const uint8_t *data = (const uint8_t *) data2;
	uint8_t sum = 0;

	size_t i;
	for(i = 0; i < len; ++i)
		sum = sum ^ data[i];

	return sum;
}

