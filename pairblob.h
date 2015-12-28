#ifndef _PAIR_BLOB_H
#define _PAIR_BLOB_H

#include <cstddef>	// offsetof
#include <cstring>
#include <cstdint>

#include <endian.h>

class Pair;

struct PairBlob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[1];	// dynamic

	// ==============================

public:
	friend class Pair;

	static constexpr uint16_t MAX_KEY_SIZE = 0xffff;
	static constexpr uint32_t MAX_VAL_SIZE = 0xffffffff;

	// ==============================

private:
	static PairBlob *create(const char *key, size_t keylen, const char *val, size_t vallen, uint32_t expires = 0, uint32_t created = 0) noexcept;
	static void destroy(PairBlob *pair) noexcept;

	static PairBlob *clone(const PairBlob *src);

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
		return key == nullptr || size == 0 ? -1 : __compare(getKey(), getKeyLen(), key, size);
	}

	int cmp(const char *key) const noexcept{
		return cmp(key, strlen(key));
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

	static int __compare(const char *s1, size_t size1, const char *s2, size_t size2) noexcept;
	static uint64_t __getCreateTime(uint32_t created) noexcept;
	static uint8_t __calcChecksum(const char *buffer, size_t size) noexcept;

} __attribute__((__packed__));

#endif

