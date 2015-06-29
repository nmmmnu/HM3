#include "pair.h"
#include "mytime.h"

#include "defs.h"

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

// ==============================

std_optional<IChecksumCalculator> Pair::_checksumCalculator = nullptr;

// ==============================

Pair *Pair::create(const char *key, const void *val, size_t vallen, uint32_t expires, uint32_t created){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE)
		return nullptr;

	size_t size = __sizeofBase() + keylen + 1 + vallen + 1;

	Pair *p = (Pair *) xmalloc(size);

	if (p == NULL)
		return nullptr;

	p->created	= htobe64(__getCreateTime(created));
	p->expires	= htobe32(expires);
	p->vallen	= htobe32(vallen);
	p->keylen	= htobe16(keylen);

	// memcpy so we can switch to blobs later.
	memcpy(& p->buffer[0], key, keylen);
	p->buffer[keylen] = '\0';

	// this is safe with NULL pointer.
	memcpy(& p->buffer[keylen + 1], val, vallen);
	p->buffer[keylen + 1 + vallen] = '\0';

	p->checksum = p->_getChecksum();

	// p must be valid POD class
	return p;
}

void Pair::destroy(const Pair *pair){
	xfree((void *) pair);
}

// ==============================

const char *Pair::getKey() const{
	return & buffer[0];
}

const char *Pair::getVal() const{
	// vallen is 0 no matter of endianness
	if (vallen == 0)
		return NULL;

	return & buffer[ be16toh(keylen) + 1 ];
}

bool Pair::valid() const{
	// now expires is 0 no matter of endianness
	if (expires){
		if ( MyTime::expired( be64toh(created), be32toh(expires) ) )
			return false;
	}

	// now check checksum
	if (_checksumCalculator){
		if (checksum != _getChecksum())
			return false;
	}

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

void Pair::print() const{
	static const char *format = "%-20s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(created)),
		be32toh(expires)
	);
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t created){
	return created ? MyTime::combine(created) : MyTime::now();
}

uint8_t Pair::_getChecksum() const{
	if (! _checksumCalculator)
		return 0;

	return _checksumCalculator.value().calcChecksum(buffer, _sizeofBuffer());
}

constexpr size_t Pair::__sizeofBase(){
	return offsetof(Pair, buffer);
}

size_t Pair::_sizeofBuffer() const{
	return be16toh(keylen) + 1 + be32toh(vallen) + 1;
}

