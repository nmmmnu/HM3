#include "pair.h"
#include "mytime.h"

#include <new>		// new (std::nothrow)

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include "defs.h"

// ==============================

struct PairBlob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[2];	// dynamic, at least 2
} __attribute__((__packed__));

// ==============================

std_optional<IChecksumCalculator> Pair::_checksumCalculator = NULL;

// ==============================

const void *Pair::__createBuffer(const char *key, const void *val, size_t vallen, uint32_t expires){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE)
		return nullptr;

	size_t sizeBuffer = keylen + 1 + vallen + 1;

	PairBlob *p = (PairBlob*) new (std::nothrow) char[__sizeofBase() + sizeBuffer];

	if (p == nullptr)
		return nullptr;

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

	p->checksum = __getChecksum(p->buffer, sizeBuffer);

	return p;
}

// ==============================

const char *Pair::getKey() const{
	return & _getBlob()->buffer[0];
}

const char *Pair::getVal() const{
	// vallen is 0 no matter of endianness
	if (_getBlob()->vallen == 0)
		return nullptr;

	return & _getBlob()->buffer[ be16toh(_getBlob()->keylen) + 1 ];
}

int Pair::cmp(const char *key) const{
	return key == nullptr ? -1 : strcmp(getKey(), key);
}

bool Pair::valid() const{
	// now expires is 0 no matter of endianness
	if (_getBlob()->expires){
		if ( MyTime::expired( be64toh(_getBlob()->created), be32toh(_getBlob()->expires) ) )
			return false;
	}

	// now check checksum
	if (_checksumCalculator){
		if (_getBlob()->checksum != __getChecksum(_getBlob()->buffer, _sizeofBuffer()))
			return false;
	}

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

void Pair::print() const{
	static const char *format = "%-10s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(_getBlob()->created)),
		be32toh(_getBlob()->expires)
	);
}

// ==============================

size_t Pair::_sizeofBuffer() const{
	return be16toh(_getBlob()->keylen) + 1 + be32toh(_getBlob()->vallen) + 1;
}

constexpr size_t Pair::__sizeofBase(){
	return offsetof(PairBlob, buffer);
}

uint8_t Pair::__getChecksum(const void *buffer, size_t size){
	return _checksumCalculator ?_checksumCalculator.value().calcChecksum(buffer, size) : 0;
}

// ==============================

