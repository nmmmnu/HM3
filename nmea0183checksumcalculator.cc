#include "nmea0183checksumcalculator.h"

#include <stdio.h>

uint8_t NMEA0183ChecksumCalculator::calcChecksum(const void *data2, size_t size){
	// NMEA 0183 like checksum
	const uint8_t *data = (const uint8_t *) data2;
	uint8_t sum = 0;

	size_t i;
	for(i = 0; i < size; ++i)
		sum = sum ^ data[i];

	return sum;
}

