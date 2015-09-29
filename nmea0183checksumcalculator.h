#ifndef _NMEA_0183_CHECKSUM_H
#define _NMEA_0183_CHECKSUM_H

#include "ichecksumcalculator.h"

class NMEA0183ChecksumCalculator final : virtual public IChecksumCalculator{
private:
	uint8_t _calc(const void *data2, size_t len, uint8_t start = 0) noexcept final;
};

#endif

