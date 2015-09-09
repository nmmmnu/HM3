#include "pair.h"

#include <stdexcept>

#include "pairpod.h"

// ==============================

Pair::Pair(const char *key, const void *val, size_t const vallen, uint32_t const expires, uint32_t const created):
		created(__getCreateTime(created)),
		expires(expires),
		key(key),
		val( (const char *) val, vallen)
{
	size_t const keylen = strlen(key);

	if (keylen == 0){
		std::logic_error exception("Key is zero size");
		throw exception;
	}

	if (keylen > MAX_KEY_SIZE || vallen > MAX_VAL_SIZE){
		std::logic_error exception("Key or value size too big");
		throw exception;
	}
}

Pair::Pair(const char *key, const char *value, uint32_t expires, uint32_t created) :
			Pair(key, value, value ? strlen(value) : 0, expires, created){}

Pair::Pair(const void *blob2){
	if (blob2 == nullptr)
		return;

	PairPOD *blob = (PairPOD *) blob2;

	if (blob->valid() == false)
		return;

	const auto keylen = be16toh(blob->keylen);
	
	if (keylen == 0 || keylen > MAX_KEY_SIZE)
		return;

	const auto vallen = be32toh(blob->vallen);

	if (vallen > MAX_VAL_SIZE)
		return;

	created = be64toh(blob->created);
	expires = be32toh(blob->expires);

	key = std::string(blob->getKey(), keylen);
	val = std::string(blob->getVal(), vallen);

}

// ==============================

bool Pair::fwrite(std::ostream & os) const{
	PairPOD p;

	p.created	= htobe64(created);
	p.expires	= htobe32(expires);
	p.vallen	= htobe32(val.size());
	p.keylen	= htobe16(key.size());

	NMEA0183ChecksumCalculator chk;

	chk.add(key.c_str(), key.size());
	chk.add("\0", 1);
	chk.add(val.c_str(), val.size());
	chk.add("\0", 1);

	p.checksum	= chk.get();

	os.write((const char *) & p, p.__sizeofBase() );
	os.write(key.c_str(), key.size());
	os.write("\0", 1);
	os.write(val.c_str(), val.size());
	os.write("\0", 1);

	return true;
}

void Pair::print() const{
	if (*this == false){
		printf("--- Pair is empty ---\n");
		return;
	}

	static const char *format = "%-20s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey(), getVal(),
		MyTime::STRING_SIZE, MyTime::toString(created),
		expires
	);
}

size_t Pair::getSize() const{
	return PairPOD::__sizeofBase() + key.size() + 1 + val.size() + 1;
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t const created){
	return created ? MyTime::combine(created) : MyTime::now();
}

