#ifndef _MYTIME_H
#define _MYTIME_H

#include <stdint.h>
#include <stdlib.h>	// size_t

class MyTime{
public:
	static const size_t STRING_SIZE = 20;

public:
	static const char *toString(uint64_t date);

	static uint64_t now();

	inline static bool expired(uint64_t date, uint32_t expiration){
		return date + combine(expiration) < now();
	}

	inline static uint64_t combine(uint32_t sec, uint32_t usec = 0){
		return (uint64_t) sec << 32 | usec;
	};

	inline static uint32_t uncombine(uint64_t timestamp){
		return timestamp >> 32;
	};

	static char buffer[STRING_SIZE];
};

#endif

