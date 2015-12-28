#include "pair.h"
#include "pairblob.h"

#include <stdexcept>

// ==============================

Pair Pair::_zero = {};

// ==============================

Pair::Pair(const StringRef &key, const StringRef &val, uint32_t const expires, uint32_t const created){
	if (key.empty()){
		std::logic_error exception("Key is zero size");
		throw exception;
	}

	pimpl = PairBlob::create(	key.data(), key.size(),
					val.data(), val.size(),
					expires, created);

	if (pimpl == nullptr){
		std::logic_error exception("Problem creating PairBlob");
		throw exception;
	}
}

Pair::Pair(const void *blob2){
	if (blob2 == nullptr)
		return;

	pimpl = PairBlob::clone( (PairBlob *) blob2 );

	if (pimpl == nullptr){
		std::logic_error exception("Problem creating PairBlob");
		throw exception;
	}
}

Pair::Pair(const Pair &other){
	pimpl = PairBlob::clone( other.pimpl );

	if (pimpl == nullptr){
		std::logic_error exception("Problem creating PairBlob");
		throw exception;
	}
}

Pair &Pair::operator=(const Pair &other){
	pimpl = PairBlob::clone( other.pimpl );

	if (pimpl == nullptr){
		std::logic_error exception("Problem creating PairBlob");
		throw exception;
	}

	return *this;
}

Pair::Pair(Pair &&other) noexcept{
	pimpl = other.pimpl;
	other.pimpl = nullptr;
}

Pair &Pair::operator=(Pair &&other) noexcept{
	pimpl = other.pimpl;
	other.pimpl = nullptr;

	return *this;
}

Pair::~Pair(){
	PairBlob::destroy(pimpl);
}

// ==============================

StringRef Pair::getKey() const noexcept{
	if (pimpl == nullptr)
		return StringRef();

	return StringRef(pimpl->getKey(), pimpl->getKeyLen());
}

StringRef Pair::getVal() const noexcept{
	if (pimpl == nullptr)
		return StringRef();

	return StringRef(pimpl->getVal(), pimpl->getValLen());
}

uint64_t Pair::getCreated() const noexcept{
	if (pimpl == nullptr)
		return 0;

	return pimpl->getCreated();
}

int Pair::cmp(const StringRef &key) const noexcept{
	if (pimpl == nullptr)
		return CMP_ZERO;

	return pimpl->cmp(key.data(), key.size());
}

bool Pair::isTombstone() const noexcept{
	if (pimpl == nullptr)
		return true;

	return pimpl->isTombstone();
}

bool Pair::valid(bool const tombstoneCheck) const noexcept{
	if (pimpl == nullptr)
		return false;

	return pimpl->valid(tombstoneCheck);
}

size_t Pair::getSize() const noexcept{
	if (pimpl == nullptr)
		return 0;

	return pimpl->getSize();
}

void Pair::print() const noexcept{
	if (pimpl == nullptr){
		printf("--- Pair is empty ---\n");
		return;
	}

	pimpl->print();
}

bool Pair::fwrite(std::ostream & os) const{
	if (pimpl == nullptr)
		return false;

	os.write((const char *) & pimpl, getSize() );

	return true;
}

