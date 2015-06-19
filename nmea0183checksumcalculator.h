#ifndef _NMEA_0183_CHECKSUM_H
#define _NMEA_0183_CHECKSUM_H

#include "ichecksumcalculator.h"

class NMEA0183ChecksumCalculator : public IChecksumCalculator{
public:
	virtual uint8_t calcChecksum(const void *data2, size_t len);
};

#endif

