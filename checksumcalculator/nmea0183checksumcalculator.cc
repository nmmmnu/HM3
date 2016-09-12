#include "nmea0183checksumcalculator.h"

namespace checksumcalculator{

auto NMEA0183ChecksumCalculator::operator()(const void *data2, size_t const size, result_type const start) const noexcept -> result_type{
	// NMEA 0183 like checksum
	const uint8_t *data = (const uint8_t *) data2;
	uint8_t sum = start;

	for(size_t i = 0; i < size; ++i)
		sum = sum ^ data[i];

	return sum;
}

} // namespace

//template class Checksum<NMEA0183ChecksumCalculator>;
