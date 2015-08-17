#ifndef _ICHECKSUM_CALCULATOR_H
#define _ICHECKSUM_CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <string.h>	// strlen

class IChecksumCalculator{
public:
	virtual ~IChecksumCalculator(){};

	uint8_t calc(const void *data, size_t size);

	void clear();
	uint8_t add(const void *data, size_t size);
	uint8_t get();

private:
	virtual uint8_t _calc(const void *data, size_t size, uint8_t start = 0) = 0;

private:
	uint8_t _psum = 0;
};


inline uint8_t IChecksumCalculator::calc(const void *data, size_t const size){
	return _calc(data, size, 0);
}

inline void IChecksumCalculator::clear(){
	_psum = 0;
}

inline uint8_t IChecksumCalculator::add(const void *data, size_t const size){
	_psum = _calc(data, size, _psum);
	return _psum;
}

inline uint8_t IChecksumCalculator::get(){
	return _psum;
}

#endif
