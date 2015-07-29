#include "pair.h"
#include "mytime.h"

#include <endian.h>	// htobe16
#include <stddef.h>	// offsetof

#include <stdexcept>

// ==============================

NMEA0183ChecksumCalculator Pair::__checksumCalculator;
bool Pair::__checksumUsage = Pair::CHECKSUM;

// ==============================

struct Pair::Blob{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[2];	// dynamic

public:
	static void *operator new(size_t size_reported, size_t size) {
		return ::operator new(size);
	}

} __attribute__((__packed__));

// ==============================

static void null_deleter(void *){
	// null_deleter
}

// ==============================

Pair::Pair(const char *key, const void *val, size_t vallen, uint32_t expires, uint32_t created){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE){
		std::logic_error exception("Key or value size too big");
		throw exception;
	}

	size_t size_buff = keylen + 1 + vallen + 1;
	size_t size      = __sizeofBase() + size_buff;

	Blob *p = new(size) Blob(); // may throw

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

	p->checksum = __checksumCalculator.calc(p->buffer, size_buff);

	_blob.reset(p);
}

Pair::Pair(const char *key, const char *value,              uint32_t expires, uint32_t created) :
			Pair(key, value, value ? strlen(value) : 0, expires, created){
}

Pair::Pair(const void *blob2, bool weak){
	if (blob2 == nullptr || weak == false)
		return;

	Blob *p = (Blob *) blob2;

	_blob.reset(p, null_deleter);
}

Pair::Pair(const void *blob2){
	if (blob2 == nullptr)
		return;

	const Blob *raw = (const Blob *) blob2;

	size_t size_buff = be16toh(raw->keylen) + 1 + be32toh(raw->vallen) + 1;
	size_t size      = __sizeofBase() + size_buff;

	if (__checksumUsage && raw->checksum != __checksumCalculator.calc(raw->buffer, size_buff))
		return;

	Blob *p = new(size) Blob();

	memcpy(p, raw, size);

	_blob.reset(p);
}

// ==============================

const char *Pair::getKey() const{
	if (! _blob)
		return nullptr;

	return _blob->buffer;
}

const char *Pair::getVal() const{
	if (! _blob)
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
	if (_blob->expires && MyTime::expired( be64toh(_blob->created), be32toh(_blob->expires) ) )
		return false;

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return __sizeofBase() + _sizeofBuffer();
}

bool Pair::fwrite(std::ostream & os) const{
	os.write( (const char *) _blob.get(), getSize() );

	return true;
}

void Pair::print() const{
	if (_blob == nullptr){
		printf("--- Pair is empty ---\n");
		return;
	}

	static const char *format = "%-20s | %-20s | %-*s | %8u | %4u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(_blob->created)),
		be32toh(_blob->expires),
		_blob.use_count()
	);
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t created){
	return created ? MyTime::combine(created) : MyTime::now();
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

