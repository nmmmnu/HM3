#include "pairblob.h"

#include "mytime.h"
#include "nmea0183checksumcalculator.h"

#include <cstdlib>	// malloc, free
#include <cstdio>



#define xmalloc(s) malloc(s)
#define xfree(p)   free(p)



PairBlob *PairBlob::create(	const char *key, size_t const keylen,
				const char *val, size_t const vallen,
				uint32_t const expires, uint32_t const created) noexcept{
	if (key == nullptr || keylen == 0)
		return nullptr;

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE)
		return nullptr;

	PairBlob *pair = (PairBlob *) xmalloc(__sizeofBase() + keylen + 1 + vallen + 1);

	if (pair == nullptr)
		return nullptr;

	pair->created	= htobe64(__getCreateTime(created));
	pair->expires	= htobe32(expires);
	pair->vallen	= htobe32((uint32_t) vallen);
	pair->keylen	= htobe16((uint16_t) keylen);

	// memcpy so we can switch to blobs later...
	memcpy(& pair->buffer[0],		key, keylen);
	pair->buffer[keylen] = '\0';

	// this is safe with NULL pointer.
	memcpy(& pair->buffer[keylen + 1],	val, vallen);
	pair->buffer[keylen + 1 + vallen] = '\0';

	pair->checksum	= pair->calcChecksum();

	return pair;
}

PairBlob *PairBlob::clone(const PairBlob *src){
	size_t const size = src->getSize();

	PairBlob *pair = (PairBlob *) xmalloc(size);

	if (pair == nullptr)
		return nullptr;

	memcpy(pair, src, size);

	return pair;
}

void PairBlob::destroy(PairBlob *pair) noexcept{
	xfree(pair);
}

// ==============================

bool PairBlob::valid(bool const tombstoneCheck) const noexcept{
	// check key size
	if (keylen == 0)
		return false;

	if (tombstoneCheck && vallen == 0)
		return false;

	// now expires is 0 no matter of endianness
	if (expires && MyTime::expired( getCreated(), be32toh(expires) ) )
		return false;

	// calc checksum
	if (calcChecksum() != checksum)
		return false;

	// finally all OK
	return true;
}

void PairBlob::print() const noexcept{
	static const char *format = "%-20s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(getCreated()),
		be32toh(expires)
	);
}

// ==============================

int PairBlob::__compare(const char *s1, size_t const size1, const char *s2, size_t const size2) noexcept{
	// Lazy based on LLVM::StringRef
	// http://llvm.org/docs/doxygen/html/StringRef_8h_source.html

	// check prefix
	if ( int res = memcmp(s1, s2, std::min(size1, size2 ) ) )
		return res < 0 ? -1 : +1;

	// prefixes match, so we only need to check the lengths.
	if (size1 == size2)
		return 0;

	return size1 < size2 ? -1 : +1;
}

uint64_t PairBlob::__getCreateTime(uint32_t const created) noexcept{
	return created ? MyTime::combine(created) : MyTime::now();
}

uint8_t PairBlob::__calcChecksum(const char *buffer, size_t const size) noexcept{
	Checksum<NMEA0183ChecksumCalculator> chk;

	return chk.calc(buffer, size);
}


