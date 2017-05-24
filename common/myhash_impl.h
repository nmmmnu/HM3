
namespace myhash{
namespace impl{
	
// anonymous namespace give problems in C++14.
// those are templates anyway, so nothing wrong will happen.

	template <class HASH>
	class MyHashBase{
	public:
		using uint = typename HASH::type;

		constexpr static uint offset_basis = HASH::offset_basis;

	public:
		uint operator()(const char *s) const{
			uint hash = offset_basis;

			for(const char *c = s; *c; ++c)
				h_.add(hash, *c);

			return hash;
		}

		uint operator()(const char *s, size_t const size) const{
			uint hash = offset_basis;

			for(const char *c = s; c < s + size; ++c)
				h_.add(hash, *c);

			return hash;
		}

	private:
		HASH h_;
	};

	// =====================================

	template <typename uint>
	struct DJB2_base_{
		using type = uint;

		constexpr static uint offset_basis = 5381u;
	};

	template <typename uint>
	struct DJB2 : public DJB2_base_<uint>{
		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) + (uint) c;
		}
	};

	template <typename uint>
	struct DJB2A : public DJB2_base_<uint>{
		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) ^ (uint) c;
		}
	};

	// =====================================

	template <typename uint>
	struct SDBM{
		using type = uint;

		constexpr static uint offset_basis = 0;

		void add(uint &hash, char const c) const{
			hash = ((hash << 5) + hash) ^ (uint) c;
		}
	};

	// =====================================

	struct NMEA0183{
		using type = uint8_t;

		constexpr static uint8_t offset_basis = 0;

		void add(uint8_t &hash, char const c) const{
			hash = hash ^ (uint8_t) c;
		}
	};

	// =====================================

	template <typename uint, uint BASIS, uint PRIME>
	struct FNV1_base_{
		using type = uint;

		constexpr static uint offset_basis = BASIS;
		constexpr static uint fnv_prime    = PRIME;

		void calc1(uint &hash, char const c) const{
			hash ^= (uint) c;
		}

		void calc2(uint &hash, char const c) const{
			hash *= fnv_prime;
		}
	};

	template <typename uint>
	struct FNV1_base2_;

	template <>
	struct FNV1_base2_<uint32_t> : public FNV1_base_<uint32_t, 2166136261ul, 16777619ul>{
		constexpr static unsigned char ZERO_HASH[] = { 0x01, 0x47, 0x6c, 0x10, 0xf3, 0x00 };

		void calc2(uint32_t &hash, char const c) const{
			(void)ZERO_HASH;

			// multiply by the 32 bit FNV_prime prime mod 2^32
			hash +=	(hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);
		}
	};

	template <>
	struct FNV1_base2_<uint64_t> : public FNV1_base_<uint64_t, 14695981039346656037ull, 1099511628211ull>{
		constexpr static unsigned char ZERO_HASH[] = { 0x92, 0x06, 0x77, 0x4c, 0xe0, 0x2f, 0x89, 0x2a, 0xd2, 0x00 };

		void calc2(uint64_t &hash, char const c) const{
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
	struct FNV1 : public FNV1_base2_<uint>{
		void add(uint &hash, char const c) const{
			this->calc1(hash, c);
			this->calc2(hash, c);
		}
	};

	template <typename uint>
	struct FNV1A : public FNV1_base2_<uint>{
		void add(uint &hash, char const c) const{
			this->calc2(hash, c);
			this->calc1(hash, c);
		}
	};

} // namespace impl
} // namespace

