#ifndef _IO_BUFFER_H
#define _IO_BUFFER_H

#include "stringref.h"

#include <cstdio>

namespace net{

class IOBuffer{
private:
	using container_type	= std::string;
	using size_type		= container_type::size_type;

private:
	size_type		head_	= 0;
	container_type		buffer_;

public:
	void clear(){
		buffer_.clear();
		head_ = 0;
	}

	// ==================================

	const char *data() const{
		const char *s = buffer_.data();
		return & s[head_];
	}

	size_t size() const{
		return buffer_.size() - head_;
	}

	// ==================================

	bool push(const char c){
		buffer_.push_back(c);
		return true;
	}

	bool push(const char *p){
		size_t size;

		// C++17 if :)
		if (p && ( size = strlen(p) ) )
			return push_(size, p);

		return false;
	}

	bool push(const StringRef &sr){
		if (sr.size())
			return push_(sr.size(), sr.data());

		return false;
	}

	bool push(size_t const len, const char *ptr){
		if (ptr && len)
			return push_(len, ptr);

		return false;
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

		head_ = head_ + len;

		return true;
	}

	// ==================================

	bool push(ssize_t const len, const char *ptr){
		return push((size_t) len, ptr);
	}

	bool pop(ssize_t const len){
		return pop((size_t) len);
	}

	// ==================================

	void print() const{
		printf("h: %3zu | s: %3zu | %.*s\n",
				head_,
				size(),
				(int) size(), buffer_.data() );
	}

private:
	bool push_(size_t const len, const char *ptr){
		if (ptr == nullptr || len == 0)
			return false;

		buffer_.append(ptr, len);

		return true;
	}
};

} // namespace

#endif

