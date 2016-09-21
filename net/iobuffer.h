#ifndef _IO_BUFFER_H
#define _IO_BUFFER_H

#include "stringref.h"

#include <vector>

#include <cstring>
#include <cstdio>

namespace net{


// ==================================

class IOBuffer{
public:
	using size_type = std::vector<char>::size_type;

private:
	size_type		head_	= 0;
	std::vector<char>	buffer_;

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
		return p ? push(strlen(p), p) : false;
	}

	bool push(const StringRef &sr){
		return push(sr.size(), sr.data());
	}

	bool push(size_t const len, const char *ptr){
		if (ptr == nullptr || len == 0)
			return false;

		for(size_t i = 0; i < len; ++i)
			buffer_.push_back( ptr[i] );

		return true;
	}

	bool push(ssize_t const len, const char *ptr){
		return push((size_t) len, ptr);
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
};

} // namespace

#endif

