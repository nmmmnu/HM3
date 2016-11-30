#include "myhash.h"

//#include <cstdio>

namespace{

	template <class HASH>
	class Hash_base_{
	public:
		using uint = typename HASH::uint;

		constexpr static uint offset_basis = HASH::offset_basis;

	public:
		uint calc(const char *s) const{
			uint hash = offset_basis;

			for(const char *c = s; *c; ++c)
				h_.add(hash, *c);

			return hash;
		}

		uint calc(const char *s, size_t const size) const{
			uint hash = offset_basis;

			for(const char *c = s; c < s + size; ++c)
				h_.add(hash, *c);

			return hash;
		}

	private:
		HASH h_;
	};



	// =====================================



	template <typename UINT>
	struct DJB2_base_{
		using uint = UINT;

		constexpr static uint offset_basis = 5381u;
	};

	template <typename uint>
	struct DJB2_ : public DJB2_base_<uint>{
		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) + (uint) c;
		}
	};

	template <typename uint>
	struct DJB2A_ : public DJB2_base_<uint>{
		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) ^ (uint) c;
		}
	};



	// =====================================



	template <typename UINT>
	struct SDBM_{
		using uint = UINT;

		constexpr static uint offset_basis = 0;

		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) ^ (uint) c;
		}
	};



	// =====================================



	template <typename UINT, UINT BASIS, UINT PRIME>
	struct FNV1_base_{
		using uint = UINT;

		constexpr static uint offset_basis = BASIS;
		constexpr static uint fnv_prime    = PRIME;

		void calc1(uint &hash, char const c) const{
			hash ^= (uint) c;
		}

		void calc2(uint &hash, char const c) const{
			hash *= fnv_prime;
		}
	};

	template <typename UINT>
	struct FNV1_base2_;

	template <>
	struct FNV1_base2_<uint32_t> : public FNV1_base_<uint32_t, 2166136261ul, 16777619ul>{
		constexpr static unsigned char ZERO_HASH[] = { 0x01, 0x47, 0x6c, 0x10, 0xf3, 0x00 };

		void calc2(uint &hash, char const c) const{
			(void)ZERO_HASH;

			// multiply by the 32 bit FNV_prime prime mod 2^32
			hash +=	(hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
		}
	};

	template <>
	struct FNV1_base2_<uint64_t> : public FNV1_base_<uint64_t, 14695981039346656037ull, 1099511628211ull>{
		constexpr static unsigned char ZERO_HASH[] = { 0x92, 0x06, 0x77, 0x4c, 0xe0, 0x2f, 0x89, 0x2a, 0xd2, 0x00 };

		void calc2(uint &hash, char const c) const{
			(void)ZERO_HASH;

			// multiply by the 64 bit FNV_prime prime mod 2^64
			hash +=	(hash << 1) + (hash << 4) + (hash << 5) +
				(hash << 7) + (hash << 8) + (hash << 40);
		}
	};

	// uint128_t
	// 144066263297769815596495629667062367629
	// 309485009821345068724781371

	template <typename uint>
	struct FNV1_ : public FNV1_base2_<uint>{
		void add(uint &hash, char const c) const{
			this->calc1(hash, c);
			this->calc2(hash, c);
		}
	};

	template <typename uint>
	struct FNV1A_ : public FNV1_base2_<uint>{
		void add(uint &hash, char const c) const{
			this->calc2(hash, c);
			this->calc1(hash, c);
		}
	};



	// =====================================



	struct NMEA0183_{
		using uint = uint8_t;

		constexpr static uint offset_basis = 0;

		void add(uint &hash, char const c) const{
			hash = hash ^ (uint) c;
		}
	};

} // namespace



// =====================================

template <typename uint>
uint DJB2Hash<uint>::operator()(const char *s) const{
	Hash_base_<DJB2_<uint> > hb;
	return hb.calc(s);
}

template <typename uint>
uint DJB2Hash<uint>::operator()(const char *s, size_t const size) const{
	Hash_base_<DJB2_<uint> > hb;
	return hb.calc(s, size);
}

// -------------------------------------

template <typename uint>
uint DJB2AHash<uint>::operator()(const char *s) const{
	Hash_base_<DJB2A_<uint>	> hb;
	return hb.calc(s);
}

template <typename uint>
uint DJB2AHash<uint>::operator()(const char *s, size_t const size) const{
	Hash_base_<DJB2A_<uint>	> hb;
	return hb.calc(s, size);
}

// -------------------------------------

template <typename uint>
uint SDBMHash<uint>::operator()(const char *s) const{
	Hash_base_<SDBM_<uint> > hb;
	return hb.calc(s);
}

template <typename uint>
uint SDBMHash<uint>::operator()(const char *s, size_t const size) const{
	Hash_base_<SDBM_<uint> > hb;
	return hb.calc(s, size);
}

// -------------------------------------

template <typename uint>
uint FNV1Hash<uint>::operator()(const char *s) const{
	Hash_base_<FNV1_<uint> > hb;

	return hb.calc(s);
}

template <typename uint>
uint FNV1Hash<uint>::operator()(const char *s, size_t const size) const{
	Hash_base_<FNV1_<uint> > hb;
	return hb.calc(s, size);
}

// -------------------------------------

template <typename uint>
uint FNV1AHash<uint>::operator()(const char *s) const{
	Hash_base_<FNV1A_<uint>	> hb;
	return hb.calc(s);
}

template <typename uint>
uint FNV1AHash<uint>::operator()(const char *s, size_t const size) const{
	Hash_base_<FNV1A_<uint>	> hb;
	return hb.calc(s, size);
}

// -------------------------------------

uint8_t NMEA0183Hash::operator()(const char *s) const{
	Hash_base_<NMEA0183_> hb;
	return hb.calc(s);
}

uint8_t NMEA0183Hash::operator()(const char *s, size_t const size) const{
	Hash_base_<NMEA0183_> hb;
	return hb.calc(s, size);
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

