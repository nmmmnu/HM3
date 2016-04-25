#ifndef _ICHECKSUM_H
#define _ICHECKSUM_H

#include <cstdlib>	// size_t

#include <memory>

template<class T>
class Checksum{
public:
	using result_type = typename T::result_type;

public:
	Checksum() = default;

	template<class CALCULATOR>
	Checksum(CALCULATOR &&calculator) : calculator_(std::forward(calculator)){}

public:
	result_type calc(const void *data, size_t size, result_type start = 0) const noexcept{
		return calculator_(data, size, start);
	}

	void clear() noexcept{
		psum_ = 0;
	}

	result_type add(const void *data, size_t size) noexcept{
		psum_ = calc(data, size, psum_);
		return psum_;
	}

	result_type get() const noexcept{
		return psum_;
	}

private:
	T		calculator_;
	result_type	psum_		= 0;
};

#endif
