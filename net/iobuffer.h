#ifndef _IO_BUFFER_H
#define _IO_BUFFER_H

#include "stringref.h"

#include <limits>

#include <cstdint>
#include <cstring>
#include <cstdio>

namespace net{

namespace iobuffer{
	using size_type = uint16_t;
}

// ==================================

template<
	size_t CAPACITY,
	bool VALID = CAPACITY < std::numeric_limits<iobuffer::size_type>::max()
>
class IOBuffer{
	static_assert(
		VALID,
		"IOBuffer invalid capacity; max capacity = std::numeric_limits<size_type>::max()"
	);
};

// ==================================

template<size_t CAPACITY>
class IOBuffer<CAPACITY, true>{
public:
	using size_type = iobuffer::size_type;

private:
	size_type	head_	= 0;
	size_type	tail_	= 0;
	char		buffer_[CAPACITY];

public:
	void clear(){
		head_ = 0;
		tail_ = 0;
	}

	constexpr
	static size_t max_size(){
		return CAPACITY;
	}

	// ==================================

	const char *data() const{
		return & buffer_[head_];
	}

	const char *dataTail() const{
		return & buffer_[tail_];
	}

	char *dataTail(){
		return & buffer_[tail_];
	}

	size_t size() const{
		return tail_ - head_;
	}

	size_t capacity() const{
		return CAPACITY - tail_;
	}

	// ==================================

	bool push(const char *p){
		return p ? push(strlen(p), p) : false;
	}

	bool push(const char c){
		return push(1, &c);
	}

	bool push(const StringRef &sr){
		return push(sr.size(), sr.data());
	}

	bool push(size_t const len, const void *ptr = nullptr){
		if (len == 0)
			return false;

		if (capacity() < len)
			return false;

		if (ptr)
			memmove(&buffer_[tail_], ptr, len);

		tail_ = (size_type) (tail_ + len);

		return true;
	}

	bool pop(size_t const len){
		if (len == 0)
			return false;

		auto const available = size();

		if (available < len)
			return false;

		if (available == len){
			clear();
			return true;
		}

		head_ = (size_type) (head_ + len);

		return true;
	}

	// ==================================

	void print() const{
		printf("h: %3u | t: %3u | ad: %3zu | ac: %3zu | %.*s\n",
				head_, tail_,
				size(), capacity(),
				(int) size(), buffer_ );
	}
};

} // namespace

#endif

