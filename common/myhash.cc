#include "myhash.h"

#include <cstdint>



// mysterious DJB const
template <typename uint>
struct DJB2_Data_{
	constexpr static uint offset_basis = 5381u;
};



template <typename uint>
uint DJB2Hash(const char *s){
	uint hash = DJB2_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c)
		hash = ((hash << 5) + hash) + (uint) *c;

	return hash;
}

template <typename uint>
uint DJB2AHash(const char *s){
	uint hash = DJB2_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c)
		hash = ((hash << 5) + hash) ^ (uint) *c;

	return hash;
}



// =====================================



template <typename uint>
uint SDBMHash(const char *s){
	uint hash = 0;

	for(const char *c = s; *c; ++c)
		hash = (hash << 6) + (hash << 16) - hash + (uint) *c;

        return hash;
}



// =====================================



template<typename uint>
struct FNV1_Data_;

template<>
struct FNV1_Data_<uint32_t>{
	using uint = uint32_t;
	static constexpr uint offset_basis	= 2166136261ul;
	static constexpr uint FNV_prime		= 16777619ul;

	static constexpr unsigned char zeroHash[] =
						{ 0x01, 0x47, 0x6c, 0x10, 0xf3, 0x00 };

	inline static void optimizedCalc(uint &hash){
		// multiply by the 32 bit FNV magic prime mod 2^32
		hash +=
			(hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
	}
};

template<>
struct FNV1_Data_<uint64_t>{
	using uint = uint64_t;
	static constexpr uint offset_basis	= 14695981039346656037ull;
	static constexpr uint FNV_prime		= 1099511628211ull;

	static constexpr unsigned char zeroHash[] =
						{ 0x92, 0x06, 0x77, 0x4c, 0xe0, 0x2f, 0x89, 0x2a, 0xd2, 0x00 };

	inline static void optimizedCalc(uint &hash){
		// multiply by the 64 bit FNV magic prime mod 2^64
		hash +=
			(hash << 1) + (hash << 4) + (hash << 5) +
			(hash << 7) + (hash << 8) + (hash << 40);
	}
};

#if 0
template<>
struct FNV1_Data_<uint128_t>{
	using uint = uint128_t;
	static constexpr uint offset_basis	= 144066263297769815596495629667062367629ull;
	static constexpr uint FNV_prime		= 309485009821345068724781371ull;

	inline static void calc(uint &hval){
		hval *= FNV_prime;
	}
};
#endif



template <typename uint>
uint FNV1Hash(const char *s){
	uint hash = FNV1_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c){
		// hash *= FNV1_Data_<uint>::FNV_prime;
		FNV1_Data_<uint>::optimizedCalc(hash);

		hash ^= uint(*c);
	}

	return hash;
}

template <typename uint>
uint FNV1AHash(const char *s){
	uint hash = FNV1_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c){
		hash ^= uint(*c);

		// hash *= FNV1_Data_<uint>::FNV_prime;
		FNV1_Data_<uint>::optimizedCalc(hash);
	}

	return hash;
}



// =====================================



template uint32_t DJB2Hash(const char *s);
template uint64_t DJB2Hash(const char *s);

template uint32_t DJB2AHash(const char *s);
template uint64_t DJB2AHash(const char *s);

template uint32_t SDBMHash(const char *s);
template uint64_t SDBMHash(const char *s);

template uint32_t FNV1Hash(const char *s);
template uint64_t FNV1Hash(const char *s);

template uint32_t FNV1AHash(const char *s);
template uint64_t FNV1AHash(const char *s);

