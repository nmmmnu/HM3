#ifndef _STD_OPTIONAL_H
#define _STD_OPTIONAL_H

#include "std_container.h"

template <class T>
class std_optional : public std_container<T>{
public:
	std_optional(T *t = nullptr)
		: std_container<T>(t){};

	// ==============================

	inline explicit operator bool() const {
		return this->ptr() != nullptr;
	}
};

#endif
