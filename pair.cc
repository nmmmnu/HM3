#include "pair.h"
#include "mytime.h"

#include "defs.h"

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include <stdexcept>

// ==============================

NMEA0183ChecksumCalculator Pair::__checksumCalculator;

// ==============================

struct Pair::Blob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[2];	// dynamic
} __attribute__((__packed__));

// ==============================

Pair::Pair(const char *key, const void *val, size_t vallen, uint32_t expires, uint32_t created){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE){
		std::logic_error exception("Key or value size too big");
		throw exception;
	}

	size_t size = __sizeofBase() + keylen + 1 + vallen + 1;

	Blob *p = (Blob *) xmalloc(size);

	if (p == nullptr){
		std::bad_alloc exception;
		throw exception;
	}

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

	p->checksum = __checksumCalculator.calc((void *) p->buffer, size - __sizeofBase());

	// p must be valid POD class
	_blob = p;
}

Pair::Pair(const char *key, const char *value,              uint32_t expires, uint32_t created) :
			Pair(key, value, value ? strlen(value) : 0, expires, created){
};

Pair::Pair(const void *blob, bool ownBlob) :
			_blob((const Blob *) blob),
			_ownBlob(ownBlob){
};

Pair::Pair(Pair && other) :
			Pair(other._blob, other._ownBlob){
	other._ownBlob = false;
};

Pair::Pair(const Pair & other){
	if (other._ownBlob)
		printf("WARNING: copy c-tor with allocation\n");

	const void *blob = other._ownBlob ? other.cloneBlob() : other._blob;

	_blob = (const Blob *) blob;
	_ownBlob = other._ownBlob;
}

Pair & Pair::operator=(Pair other){
	std::swap(_blob,    other._blob   );
	std::swap(_ownBlob, other._ownBlob);

	return *this;
}

const void *Pair::cloneBlob() const{
	if (_blob == nullptr)
		return nullptr;

	void *p = xmalloc(getSize());

	if (p == nullptr){
		std::bad_alloc exception;
		throw exception;
	}

	memcpy(p, _blob, getSize());

	return p;
}

Pair::~Pair(){
	if (_ownBlob)
		xfree((void *) _blob);
}

// ==============================

const char *Pair::getKey() const{
	if (_blob == nullptr)
		return nullptr;

	return & _blob->buffer[0];
}

const char *Pair::getVal() const{
	if (_blob == nullptr)
		return nullptr;

	// vallen is 0 no matter of endianness
	if (_blob->vallen == 0)
		return nullptr;

	return & _blob->buffer[ be16toh(_blob->keylen) + 1 ];
}

bool Pair::valid() const{
	if (_blob == nullptr)
		return false;

	// now expires is 0 no matter of endianness
	if (_blob->expires){
		if ( MyTime::expired( be64toh(_blob->created), be32toh(_blob->expires) ) )
			return false;
	}

	// now check checksum
	if (_blob->checksum != _getChecksum())
		return false;

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

void Pair::print() const{
	if (_blob == nullptr){
		printf("--- Pair is empty ---\n");
		return;
	}

	static const char *format = "%-20s | %-20s | %-*s | %8u | %s\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(_blob->created)),
		be32toh(_blob->expires),
		_ownBlob ? "Y" : "N"
	);
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t created){
	return created ? MyTime::combine(created) : MyTime::now();
}

uint8_t Pair::_getChecksum() const{
	return __checksumCalculator.calc(_blob->buffer, _sizeofBuffer());
}

constexpr
size_t Pair::__sizeofBase(){
	return offsetof(Blob, buffer);
}

size_t Pair::_sizeofBuffer() const{
	if (_blob == nullptr)
		return 0;

	return be16toh(_blob->keylen) + 1 + be32toh(_blob->vallen) + 1;
}

