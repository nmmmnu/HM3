#include "pair.h"

#include <stdexcept>

// ==============================

NMEA0183ChecksumCalculator Pair::__checksumCalculator;
bool Pair::__checksumUsage = Pair::CHECKSUM;

// ==============================

Pair::Pair(const char *key, const void *val, size_t const vallen, uint32_t const expires, uint32_t const created){
	size_t const keylen = strlen(key);

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE){
		std::logic_error exception("Key or value size too big");
		throw exception;
	}

	size_t const size_buff = keylen + 1 + vallen + 1;
	size_t const size      = PairPOD::__sizeofBase() + size_buff;

	PairPOD *p = _createBlob(size); // may throw

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

	pod.reset(p);
}

Pair::Pair(const char *key, const char *value, uint32_t expires, uint32_t created) :
			Pair(key, value, value ? strlen(value) : 0, expires, created){}

Pair::Pair(std::shared_ptr<const PairPOD> && pod) : pod(pod){}

Pair::Pair(const void *blob2){
	if (blob2 == nullptr)
		return;

	PairPOD *blob = (PairPOD *) blob2;

	pod.reset( _cloneBlob(blob) );
}

PairPOD *Pair::_cloneBlob(const PairPOD *blob){
	if (blob == nullptr)
		return nullptr;

	size_t const size_buff = blob->_sizeofBuffer();
	size_t const size      = blob->__sizeofBase() + size_buff;

	if (__checksumUsage && blob->checksum != __checksumCalculator.calc(blob->buffer, size_buff))
		return nullptr;

	PairPOD *p = _createBlob(size); // may throw

	memcpy(p, blob, size);

	return p;
}

PairPOD *Pair::_createBlob(size_t const size){
	// allocate memory
	void *addr = ::operator new(size); // may throw

	// placement new
	return new(addr) PairPOD();
}

// ==============================

bool Pair::fwrite(std::ostream & os) const{
	os.write( (const char *) pod.get(), pod->getSize() );

	return true;
}

void Pair::print() const{
	if (pod == nullptr){
		printf("--- Pair is empty ---\n");
		return;
	}

	static const char *format = "%-20s | %-20s | %-*s | %8u | %4u\n";

	printf(format,
		pod->getKey(), pod->getVal(),
		MyTime::STRING_SIZE, MyTime::toString(be64toh(pod->created)),
		be32toh(pod->expires),
		pod.use_count()
	);
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t const created){
	return created ? MyTime::combine(created) : MyTime::now();
}

