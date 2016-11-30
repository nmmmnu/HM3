#include "myhash.h"

#include <cstdint>



namespace{

	// mysterious DJB const
	template <typename uint>
	struct DJB2_{
		constexpr static uint offset_basis = 5381u;
	};

} // namespace



template <typename uint>
uint DJB2Hash<uint>::operator()(const char *s) const{
	uint hash = DJB2_<uint>::offset_basis;

	for(const char *c = s; *c; ++c)
		hash = ((hash << 5) + hash) + (uint) *c;

	return hash;
}

template <typename uint>
uint DJB2AHash<uint>::operator()(const char *s) const{
	uint hash = DJB2_<uint>::offset_basis;

	for(const char *c = s; *c; ++c)
		hash = ((hash << 5) + hash) ^ (uint) *c;

	return hash;
}



// =====================================



template <typename uint>
uint SDBMHash<uint>::operator()(const char *s) const{
	uint hash = 0;

	for(const char *c = s; *c; ++c)
		hash = (hash << 6) + (hash << 16) - hash + (uint) *c;

        return hash;
}



// =====================================



namespace {

	template<typename uint>
	struct FNV1_Data_;

	template<>
	struct FNV1_Data_<uint32_t>{
		using uint = uint32_t;

		static constexpr uint offset_basis	= 2166136261ul;
		static constexpr uint FNV_prime		= 16777619ul;

		static constexpr bool hasOptimization	= true;

		static constexpr unsigned char ZERO_HASH[] =
							{ 0x01, 0x47, 0x6c, 0x10, 0xf3, 0x00 };

		static void optimizedMultiplication(uint &hash){
			(void)ZERO_HASH;

			// multiply by the 32 bit FNV_prime prime mod 2^32
			hash +=
				(hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
		}
	};

	template<>
	struct FNV1_Data_<uint64_t>{
		using uint = uint64_t;

		static constexpr uint offset_basis	= 14695981039346656037ull;
		static constexpr uint FNV_prime		= 1099511628211ull;

		static constexpr bool hasOptimization	= true;

		static constexpr unsigned char ZERO_HASH[] =
							{ 0x92, 0x06, 0x77, 0x4c, 0xe0, 0x2f, 0x89, 0x2a, 0xd2, 0x00 };

		static void optimizedMultiplication(uint &hash){
			(void)ZERO_HASH;

			// multiply by the 64 bit FNV_prime prime mod 2^64
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

		static constexpr bool hasOptimization	= false;

		static void optimizedMultiplication(uint &){
		}
	};
	#endif
} // namespace



template <typename uint>
uint FNV1Hash<uint>::operator()(const char *s) const{
	uint hash = FNV1_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c){
		if (FNV1_Data_<uint>::hasOptimization)
			FNV1_Data_<uint>::optimizedMultiplication(hash);
		else
			hash *= FNV1_Data_<uint>::FNV_prime;

		hash ^= uint(*c);
	}

	return hash;
}

template <typename uint>
uint FNV1AHash<uint>::operator()(const char *s) const{
	uint hash = FNV1_Data_<uint>::offset_basis;

	for(const char *c = s; *c; ++c){
		hash ^= uint(*c);

		if (FNV1_Data_<uint>::hasOptimization)
			FNV1_Data_<uint>::optimizedMultiplication(hash);
		else
			hash *= FNV1_Data_<uint>::FNV_prime;
	}

	return hash;
}



// =====================================



template struct DJB2Hash<uint32_t>;
template struct DJB2Hash<uint64_t>;

template struct DJB2AHash<uint32_t>;
template struct DJB2AHash<uint64_t>;

template struct SDBMHash<uint32_t>;
template struct SDBMHash<uint64_t>;

template struct FNV1Hash<uint32_t>;
template struct FNV1Hash<uint64_t>;

template struct FNV1AHash<uint32_t>;
template struct FNV1AHash<uint64_t>;

