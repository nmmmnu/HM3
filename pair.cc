#include "pair.h"
#include "mytime.h"

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include "defs.h"

// ==============================

std_optional<IChecksumCalculator> Pair::_checksumCalculator = NULL;

// ==============================

Pair *Pair::create(const char *key, const void *val, size_t vallen, uint32_t expires){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE)
		return NULL;

	size_t size = __sizeofPair() + keylen + 1 + vallen + 1;

	Pair *p = (Pair *) xmalloc(size);

	if (p == NULL)
		return NULL;

	p->created	= htobe64(MyTime::now());
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

void Pair::destroy(Pair *pair){
	xfree(pair);
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

bool Pair::valid(const Pair &pair) const{
	return valid();
}

bool Pair::valid(const Pair *pair) const{
	return valid();
}

size_t Pair::getSize() const{
	return __sizeofPair() + _sizeofBuffer();
}

bool Pair::writeToFile(FILE *F) const{
	size_t size = getSize();

	size_t res = fwrite(this, size, 1, F);

	return res < size;
}

void Pair::print() const{
	static const char *format = "%-10s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(created)),
		be32toh(expires)
	);
}

// ==============================

uint8_t Pair::_getChecksum() const{
	return _checksumCalculator ?_checksumCalculator().calcChecksum(buffer, _sizeofBuffer()) : 0;
}

size_t Pair::_sizeofBuffer() const{
	return be16toh(keylen) + 1 + be32toh(vallen) + 1;
}

constexpr size_t Pair::__sizeofPair(){
	return offsetof(Pair, buffer);
}

// ==============================

