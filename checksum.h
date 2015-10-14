#ifndef _ICHECKSUM_H
#define _ICHECKSUM_H

#include <stdlib.h>	// size_t

#include <memory>

template<class T>
class Checksum{
public:
	typedef typename T::result_type result_type;

public:
	Checksum() = default;

	template<class CALCULATOR>
	Checksum(CALCULATOR &&calculator) : _calculator(std::forward(calculator)){}

public:
	result_type calc(const void *data, size_t size, uint8_t start = 0) const noexcept{
		return _calculator(data, size, start);
	}

	void clear() noexcept{
		_psum = 0;
	}

	result_type add(const void *data, size_t size) noexcept{
		_psum = calc(data, size, _psum);
		return _psum;
	}

	result_type get() const noexcept{
		return _psum;
	}

private:
	T		_calculator;
	result_type	_psum		= 0;
};

#endif
