#include "pair.h"

#include <stdexcept>

#include "pairpod.h"

// ==============================

Pair::Pair(const StringRef &key2, const StringRef &val2, uint32_t const expires, uint32_t const created):
		created(__getCreateTime(created)),
		expires(expires),
		key(key2),
		val(val2)
{
	if (key.empty()){
		std::logic_error exception("Key is zero size");
		throw exception;
	}

	if (key.size() > MAX_KEY_SIZE || val.size() > MAX_VAL_SIZE){
		std::logic_error exception("Key or value size too big");
		throw exception;
	}
}

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

	if (vallen > 0)
		val = std::string(blob->getVal(), vallen);
}

// ==============================

void Pair::swap(Pair &other) noexcept{
	using std::swap;

	swap(created, other.created);
	swap(expires, other.expires);

	key.swap(other.key);
	val.swap(other.val);
}

bool Pair::fwrite(std::ostream & os) const{
	PairPOD p;

	p.created	= htobe64(created);
	p.expires	= htobe32(expires);
	p.vallen	= htobe32(val.size());
	p.keylen	= htobe16(key.size());

	NMEA0183ChecksumCalculator chk;

	chk.add(key.data(), key.size());
	chk.add("\0", 1);
	chk.add(val.data(), val.size());
	chk.add("\0", 1);

	p.checksum	= chk.get();

	os.write((const char *) & p, p.__sizeofBase() );
	os.write(key.data(), key.size());
	os.write("\0", 1);
	os.write(val.data(), val.size());
	os.write("\0", 1);

	return true;
}

void Pair::print() const noexcept{
	if (*this == false){
		printf("--- Pair is empty ---\n");
		return;
	}

	static const char *format = "%-20s | %-20s | %-*s | %8u\n";

	printf(format,
		getKey().data(), getVal().data(),
		MyTime::STRING_SIZE, MyTime::toString(created),
		expires
	);
}

size_t Pair::getSize() const noexcept{
	return PairPOD::__sizeofBase() + key.size() + 1 + val.size() + 1;
}

// ==============================

uint64_t Pair::__getCreateTime(uint32_t const created) noexcept{
	return created ? MyTime::combine(created) : MyTime::now();
}

