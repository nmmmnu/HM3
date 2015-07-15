#include "pair.h"
#include "mytime.h"

#include "defs.h"

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include <stdexcept>

// ==============================

std_optional<IChecksumCalculator> Pair::__checksumCalculator = nullptr;

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

	p->checksum = __getChecksum(p->buffer, size - __sizeofBase());

	// p must be valid POD class
	blob = p;
}

const Pair & Pair::operator=(const Pair& other){
	if (this == &other)
		return *this;
		
	if (ownBlob)
		xfree((void *) blob);
	
	blob = (const Blob *) other.cloneBlob();
	ownBlob = true;
	
	return *this;
}

const void *Pair::cloneBlob() const{
	Blob *p = (Blob *) xmalloc(getSize());
	
	if (p == nullptr){
		std::bad_alloc exception;
		throw exception;
	}

	memcpy(p, blob, getSize());
	
	return p;
}

Pair::~Pair(){
	if (ownBlob)
		xfree((void *) blob);
}

// ==============================

const char *Pair::getKey() const{
	if (blob == nullptr)
		return nullptr;
		
	return & blob->buffer[0];
}

const char *Pair::getVal() const{
	if (blob == nullptr)
		return nullptr;
		
	// vallen is 0 no matter of endianness
	if (blob->vallen == 0)
		return nullptr;

	return & blob->buffer[ be16toh(blob->keylen) + 1 ];
}

bool Pair::valid() const{
	if (blob == nullptr)
		return false;

	// now expires is 0 no matter of endianness
	if (blob->expires){
		if ( MyTime::expired( be64toh(blob->created), be32toh(blob->expires) ) )
			return false;
	}

	// now check checksum
	if (__checksumCalculator){
		if (blob->checksum != _getChecksum())
			return false;
	}

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

void Pair::print() const{
	if (blob == nullptr)
		return;

	static const char *format = "%-20s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(blob->created)),
		be32toh(blob->expires)
	);
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t created){
	return created ? MyTime::combine(created) : MyTime::now();
}

uint8_t Pair::_getChecksum() const{
	return __getChecksum(blob->buffer, _sizeofBuffer());
}

uint8_t Pair::__getChecksum(const void *buffer, size_t size){
	if (! __checksumCalculator)
		return 0;

	return __checksumCalculator.value().calcChecksum(buffer, size);
}

constexpr size_t Pair::__sizeofBase(){
	return offsetof(Blob, buffer);
}

size_t Pair::_sizeofBuffer() const{
	return be16toh(blob->keylen) + 1 + be32toh(blob->vallen) + 1;
}

