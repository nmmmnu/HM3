#include "pair.h"
#include "mytime.h"

#include <endian.h>	// htobe16

#define xmalloc(s) malloc(s)
#define xfree(p)   free(p)

typedef struct _PairBuffer{
	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.
	uint32_t	vallen;		// 4
	uint16_t	keylen;		// 2
	uint8_t		checksum;	// 1
	char		buffer[];	// dynamic
} __attribute__((__packed__)) PairBuffer;

// ==============================

Pair::Pair(IChecksumCalculator *checksumCalculator){
	this->_checksumCalculator = checksumCalculator;

	_clear();
}

Pair::~Pair(){
	destroy();
}

// ==============================

bool Pair::create(const char *key, const void *val, size_t vallen, uint32_t expires){
	size_t keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE)
		return false;

	size_t size = sizeof(PairBuffer) + keylen + 1 + vallen + 1;
	_pb = (PairBuffer *) xmalloc(size);

	if (_pb == NULL){
		_clear();
		return false;
	}

	_pbDestroy = true;

	_pb->created	= htobe64(MyTime::now());
	_pb->expires	= htobe32(expires);
	_pb->vallen	= htobe32(vallen);
	_pb->keylen	= htobe16(keylen);

	// memcpy so we can switch to blobs later.
	memcpy(& _pb->buffer[0], key, keylen);
	_pb->buffer[keylen] = '\0';

	// this is safe with NULL pointer.
	memcpy(& _pb->buffer[keylen + 1], val, vallen);
	_pb->buffer[keylen + 1 + vallen] = '\0';

	_pb->checksum = _getChecksum();

	return true;
}

bool Pair::create(const char *key, const char *value, uint32_t expires){
	return create(key, value, value ? strlen(value) : 0, expires);
}

bool Pair::create(const char *key, uint32_t expires){
	return create(key, NULL, 0, expires);
}

void Pair::destroy(){
	if (_pbDestroy)
		xfree(_pb);

	_clear();
}

void Pair::load(PairBuffer *pb){
	_pb = pb;
	_pbDestroy = false;
}

// ==============================

const char *Pair::getKey() const{
	return & _pb->buffer[0];
}

const char *Pair::getVal() const{
	// _pb->vallen is 0 no matter of endianness
	if (_pb->vallen == 0)
		return NULL;

	return & _pb->buffer[ be16toh(_pb->keylen) + 1 ];
}

bool Pair::valid() const{
	if (_pb == NULL)
		return false;

	// now expires
	if (_pb->expires){
		if ( MyTime::expired( be64toh(_pb->created), be32toh(_pb->expires) ) )
			return false;
	}

	// now check checksum
	if (_checksumCalculator){
		if (_pb->checksum != _getChecksum())
			return false;
	}

	// finally all OK
	return true;
}

size_t Pair::getSize() const{
	return sizeof(PairBuffer) + _sizeofBuffer();
}

void Pair::print() const{
	static const char *format = "%-10s | %-20s | %-*s | %8u\n";

	printf(format, getKey(), getVal(), MyTime::STRING_SIZE, MyTime::toString(be64toh(_pb->created)), be32toh(_pb->expires));
}

bool Pair::saveToFile(FILE *F) const{
	size_t size = getSize();

	size_t res = fwrite(_pb, size, 1, F);

	return res < size;
}

// ==============================

uint8_t Pair::_getChecksum() const{
	if (_checksumCalculator == NULL)
		return 0;

	return _checksumCalculator->calcChecksum(_pb->buffer, _sizeofBuffer() );
}

size_t Pair::_sizeofBuffer() const{
	return be16toh(_pb->keylen) + 1 + be32toh(_pb->vallen) + 1;
}

void Pair::_clear(){
	_pb = NULL;
	_pbDestroy = false;
}

