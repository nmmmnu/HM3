#ifndef _MYTIME_H
#define _MYTIME_H

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <string.h>	// strlen

class MyTime{
public:
	constexpr static const size_t STRING_SIZE		= 20;	// strlen("1980-01-01 00:00:00");

	constexpr static const char *DATA_FORMAT_STANDARD	= "%Y-%m-%d %H:%M:%S";
	constexpr static const char *DATA_FORMAT_NUMBER		= "%Y%m%d.%H%M%S";

public:
	static const char *toString(uint64_t date, const char *format = DATA_FORMAT_STANDARD) noexcept;

	static uint64_t now() noexcept;

	static bool expired(uint64_t date, uint32_t expiration) noexcept;

	static uint64_t combine(uint32_t sec, uint32_t usec = 0) noexcept;

	static uint32_t uncombine(uint64_t timestamp) noexcept;

private:
	static char buffer[STRING_SIZE];

};

// ==============================

inline bool MyTime::expired(uint64_t const date, uint32_t const expiration) noexcept{
	return date + combine(expiration) < now();
}

inline uint64_t MyTime::combine(uint32_t const sec, uint32_t const usec) noexcept{
	return (uint64_t) sec << 32 | usec;
};

inline uint32_t MyTime::uncombine(uint64_t const timestamp) noexcept{
	return timestamp >> 32;
};


#endif

