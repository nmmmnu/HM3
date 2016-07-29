#ifndef _CHECKED_VALUE_H
#define _CHECKED_VALUE_H

//#include <type_traits>
#include <utility>

template<class T, class STATUS, STATUS SUCCESS>
class CheckedValue{
	static_assert(std::is_pod<STATUS>::value, "STATUS must be POD type");

public:
	constexpr CheckedValue(STATUS const status) noexcept:
			status_(status){}

	constexpr CheckedValue(T &&value) :
			CheckedValue(SUCCESS, std::move(value)){}

	constexpr CheckedValue(const T &value) :
			CheckedValue(SUCCESS, value){}

	template<class UT>
	constexpr CheckedValue(STATUS const status, UT &&value) :
			status_(status),
			value_(std::forward<UT>(value)){}

public:
	operator bool() const noexcept{
		return status_ == SUCCESS;
	}

public:
	STATUS code() const noexcept{
		return status_;
	}

	T &get() noexcept{
		return value_;
	}

	const T &get() const noexcept{
		return value_;
	}

private:
	STATUS	status_;
	T	value_;
};


#endif

