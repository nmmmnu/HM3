#ifndef _PAIR_H
#define _PAIR_H

#include "stringref.h"

#include "mytime.h"
#include "stringref.h"

#include <string>
#include <ostream>
#include <memory>

// ==============================

namespace hm3{

struct PairBlob;

class Pair{
public:
	static constexpr int CMP_ZERO = +1;

private:
	using Blob = PairBlob;

public:
	Pair() = default;
	Pair(const StringRef &key, const StringRef &val, uint32_t expires = 0, uint32_t created = 0);
	Pair(const Blob *blob);

	static Pair tombstone(const StringRef &key){
		return Pair(key, StringRef{} );
	}

	Pair(const Pair &other);
	Pair &operator=(const Pair &other);

	Pair(Pair &&other); /* = default */
	Pair &operator=(Pair &&other); /* = default */

	~Pair(); /* = default */

	operator bool() const noexcept{
		return pimpl != nullptr;
	}

public:
	StringRef getKey() const noexcept;
	StringRef getVal() const noexcept;

	uint64_t getCreated() const noexcept;

	int cmp(const char *key, size_t size) const noexcept;
	int cmp(const char *key) const noexcept;

	int cmp(const StringRef &key) const noexcept{
		return cmp(key.data(), key.size());
	}

	int cmp(const Pair &pair) const noexcept{
		return cmp( pair.getKey() );
	}

	int cmpTime(const Pair &pair) const noexcept;

	bool isTombstone() const noexcept;

	bool valid(bool tombstoneCheck = false) const noexcept;

	bool valid(const Pair &pair, bool tombstoneCheck = false) const noexcept{
		return valid(tombstoneCheck);
	}

	size_t getBytes() const noexcept;

public:
	bool fwrite(std::ostream & os) const;

	void print() const noexcept;

	void swap(Pair &other){
		std::swap(pimpl, other.pimpl);
	}

public:
	struct Comp{
		template <typename T>
		bool operator()(const Pair &p, const T &rhs){
			return p.cmp(rhs) < 0;
		}
	};

public:
	constexpr
	static const Pair &zero(){
		return zero_;
	}

private:
	std::unique_ptr<Blob>	pimpl;

private:
	static const Pair		zero_;
};

// ==============================

inline void swap(Pair &p1, Pair &p2){
	p1.swap(p2);
}


} // namespace

#endif
