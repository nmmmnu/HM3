#ifndef _DJB_HASH_H
#define _DJB_HASH_H

#include <cstdint>
#include <cstdlib>	// size_t

// DJB Hash function from CDB
template <typename uint>
struct DJB2Hash{
	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};

template <typename uint>
struct DJB2AHash{
	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};



// SDBM is reimplementation of NDBM
template <typename uint>
struct SDBMHash{
	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};



// FNV1 http://www.isthe.com/chongo/tech/comp/fnv/index.html
template <typename uint>
struct FNV1Hash{
	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};

template <typename uint>
struct FNV1AHash{
	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};



// NMEA0183
struct NMEA0183Hash{
	using uint = uint8_t;

	uint operator()(const char *s) const;
	uint operator()(const char *s, size_t const size) const;
};

#endif

