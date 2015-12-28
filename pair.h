#ifndef _PAIR_H
#define _PAIR_H

#include "stringref.h"

#include "mytime.h"
#include "stringref.h"

#include <string>
#include <ostream>
//#include <utility>	// swap

// ==============================

class PairBlob;

class Pair{
private:
	constexpr static int CMP_ZERO = +1;

public:
	Pair() = default;
	Pair(const StringRef &key, const StringRef &val, uint32_t expires = 0, uint32_t created = 0);
	Pair(const void *blob);

	static Pair tombstone(const StringRef &key){
		return Pair(key, StringRef{} );
	}

	Pair(const Pair &other);
	Pair &operator=(const Pair &other);

	Pair(Pair &&other) noexcept;
	Pair &operator=(Pair &&other) noexcept;

	~Pair();

	operator bool() const noexcept{
		return pimpl != nullptr;
	}

public:
	StringRef getKey() const noexcept;
	StringRef getVal() const noexcept;

	uint64_t getCreated() const noexcept;

	int cmp(const StringRef &key) const noexcept;

	int cmp(const char *key) const noexcept{
		return cmp( StringRef{key} );
	}

	int cmp(const Pair &pair) const noexcept{
		return cmp( pair.getKey() );
	}

	bool isTombstone() const noexcept;

	bool valid(bool tombstoneCheck = false) const noexcept;

	bool valid(const Pair &pair, bool tombstoneCheck = false) const noexcept{
		return valid(tombstoneCheck);
	}

	size_t getSize() const noexcept;

public:
	static const Pair &zero(){
		return _zero;
	}

public:
	bool fwrite(std::ostream & os) const;

	void print() const noexcept;

private:
	PairBlob	*pimpl = nullptr;

private:
	static Pair	_zero;
};

#endif
