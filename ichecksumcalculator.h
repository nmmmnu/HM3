#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <string.h>	// strlen

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator() = default;

	uint8_t calc(const void *data, size_t size) noexcept;

	void clear() noexcept;
	uint8_t add(const void *data, size_t size) noexcept;
	uint8_t get() const noexcept;

private:
	virtual uint8_t _calc(const void *data, size_t size, uint8_t start = 0) noexcept = 0;

private:
	uint8_t _psum = 0;
};


inline uint8_t IChecksumCalculator::calc(const void *data, size_t const size) noexcept{
	return _calc(data, size, 0);
}

inline void IChecksumCalculator::clear() noexcept{
	_psum = 0;
}

inline uint8_t IChecksumCalculator::add(const void *data, size_t const size) noexcept{
	_psum = _calc(data, size, _psum);
	return _psum;
}

inline uint8_t IChecksumCalculator::get() const noexcept{
	return _psum;
}

#endif
