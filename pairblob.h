#ifndef _PAIR_BLOB_H
#define _PAIR_BLOB_H

#include <cstddef>	// offsetof
#include <cstring>
#include <cstdint>

#include <endian.h>

#include <memory>

#include "stringref.h"

struct PairBlob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[1];	// dynamic

	// ==============================

private:
	static constexpr uint16_t MAX_KEY_SIZE = 0xffff;
	static constexpr uint32_t MAX_VAL_SIZE = 0xffffffff;

	static constexpr int      CMP_NULLKEY  = -1;

private:
	PairBlob() = default;

	static void *operator new(size_t, size_t size, bool nothrow);

public:
	static std::unique_ptr<PairBlob> create(	const char *key, size_t keylen,
							const char *val, size_t vallen,
							uint32_t expires, uint32_t created);

	static std::unique_ptr<PairBlob> create(	const PairBlob *src);

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
		return key == nullptr || size == 0 ? CMP_NULLKEY : StringRef::compare(getKey(), getKeyLen(), key, size);
	}

	int cmp(const StringRef &key) const noexcept{
		return cmp(key.data(), key.size());
	}

	int cmp(const char *key) const noexcept{
		return cmp(key, key ? strlen(key) : 0);
	}

	bool valid(bool tombstoneCheck = false) const noexcept;

	size_t getSize() const noexcept{
		return __sizeofBase() + _sizeofBuffer();
	}

	uint8_t calcChecksum() const noexcept{
		return __calcChecksum(buffer, _sizeofBuffer());
	}

	// ==============================

	void print() const noexcept;

private:
	size_t _sizeofBuffer() const noexcept{
		return getKeyLen() + 1 + getValLen() + 1;
	}

	constexpr
	static size_t __sizeofBase() noexcept{
		return offsetof(PairBlob, buffer);
	}

	// ==============================

	static uint64_t __getCreateTime(uint32_t created) noexcept;
	static uint8_t __calcChecksum(const char *buffer, size_t size) noexcept;

} __attribute__((__packed__));

#endif

