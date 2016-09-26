#ifndef _PAIR_BLOB_H
#define _PAIR_BLOB_H

#include <cstddef>	// offsetof
#include <cstring>
#include <cstdint>

#include "endian.h"

#include <memory>

#include "stringref.h"

namespace hm3{

struct PairBlob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[1];	// dynamic

	// ==============================

private:
	static constexpr uint32_t MAX_SIZE     = 256 * 1024;

	static constexpr uint16_t MAX_KEY_SIZE =   8 * 1024;
	// defined at the end
	static const     uint32_t MAX_VAL_SIZE; // = sizeofValue__();

	static constexpr int      CMP_NULLKEY  = -1;

private:
	PairBlob() = default;

	static void *operator new(size_t, size_t size, bool nothrow);

public:
	static std::unique_ptr<PairBlob> create(	const char *key, size_t keylen,
							const char *val, size_t vallen,
							uint32_t expires, uint32_t created);

	static std::unique_ptr<PairBlob> create(	const PairBlob *src);

	static std::unique_ptr<PairBlob> create(	const std::unique_ptr<PairBlob> &src){
		return create(src.get());
	}

public:
	const char *getKey() const noexcept{
		return buffer;
	}

	const char *getVal() const noexcept{
		// vallen is 0 no matter of endianness
		if (vallen == 0)
			return nullptr;

		return & buffer[ getKeyLen() + 1 ];
	}

	size_t getKeyLen() const noexcept{
		return be16toh(keylen);
	}

	size_t getValLen() const noexcept{
		return be32toh(vallen);
	}

	bool isTombstone() const noexcept{
		return vallen == 0;
	}

	uint64_t getCreated() const noexcept{
		return be64toh(created);
	}

	int cmp(const char *key, size_t const size) const noexcept{
		return StringRef::fastEmptyChar(key, size) ?
			CMP_NULLKEY :
			StringRef::compare(getKey(), getKeyLen(), key, size);
	}

	int cmp(const char *key) const noexcept{
		return StringRef::fastEmptyChar(key) ?
			CMP_NULLKEY :
			StringRef::compare(getKey(), getKeyLen(), key, strlen(key) );
	}

	int cmp(const StringRef &key) const noexcept{
		return cmp(key.data(), key.size());
	}

	bool valid(bool tombstoneCheck = false) const noexcept;

	size_t getBytes() const noexcept{
		return sizeofBase__() + sizeofBuffer__();
	}

	uint8_t calcChecksum() const noexcept{
		return calcChecksum_(buffer, sizeofBuffer__());
	}

	uint8_t validChecksum() const noexcept{
		return calcChecksum() == checksum;
	}

	// ==============================

	void print() const noexcept;

private:
	size_t sizeofBuffer__() const noexcept{
		return getKeyLen() + 1 + getValLen() + 1;
	}

	constexpr
	static size_t sizeofBase__() noexcept{
		return offsetof(PairBlob, buffer);
	}

	constexpr
	static size_t sizeofValue__()noexcept{
		return MAX_SIZE
				- MAX_KEY_SIZE
				// key and value null terminators
				- 1 - 1
				// struct members
				- sizeofBase__();
	}

	// ==============================

	static uint64_t getCreateTime_(uint32_t created) noexcept;
	static uint8_t calcChecksum_(const char *buffer, size_t size) noexcept;
} __attribute__((__packed__));

static_assert(std::is_pod<PairBlob>::value, "PairBlob must be POD type");

} // namespace

#endif

