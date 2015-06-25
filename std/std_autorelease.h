#ifndef _STD_AUTORELEASE_H
#define _STD_AUTORELEASE_H

#include "std_container.h"

template <class T>
class std_autorelease : public std_container<T>{
public:
	std_autorelease(T *t = nullptr, bool del = true)
		: std_container<T>(t), _del(del){};

	virtual ~std_autorelease(){
		if (_del)
			release();
	};

	// ==============================

	inline void release(){
		delete this->ptr();
	};

private:
	bool _del;
};


#endif

