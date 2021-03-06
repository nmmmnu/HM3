#include "pairblob.h"

#include "mytime.h"
#include "myhash.h"

#include <cstdio>

#include <stdexcept>

namespace hm3{

using ChecksumCalculator = myhash::NMEA0183;

const uint32_t PairBlob::MAX_VAL_SIZE = PairBlob::sizeofValue__();

// ==============================

std::unique_ptr<PairBlob> PairBlob::create(	const char *key, size_t const keylen,
				const char *val, size_t const vallen,
				uint32_t const expires, uint32_t const created){
	if (key == nullptr || keylen == 0){
		std::logic_error exception("Key is zero size");
		throw exception;
	}

	if (keylen > MAX_KEY_SIZE){
		std::logic_error exception("Key is very long");
		throw exception;
	}

	if (vallen > MAX_VAL_SIZE){
		std::logic_error exception("Value is very long");
		throw exception;
	}

	size_t const size = sizeofBase__() + keylen + 1 + vallen + 1;

	std::unique_ptr<PairBlob>  pair{ new(size, std::nothrow) PairBlob };

	pair->created	= htobe64(getCreateTime_(created));
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

std::unique_ptr<PairBlob> PairBlob::create(const PairBlob *src){
	if (src == nullptr)
		return {};

	size_t const size = src->getBytes();

	std::unique_ptr<PairBlob> pair{ new(size) PairBlob };

	memcpy(pair.get(), src, size);

	return pair;
}

// ==============================

bool PairBlob::valid(bool const tombstoneCheck) const noexcept{
	// check key size, keylen can not be 0
	// if (keylen == 0)
	//	return false;

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

uint64_t PairBlob::getCreateTime_(uint32_t const created) noexcept{
	return created ? MyTime::combine(created) : MyTime::now();
}

uint8_t PairBlob::calcChecksum_(const char *buffer, size_t const size) noexcept{
	ChecksumCalculator hash;

	return hash(buffer, size);
}



} // namespace
