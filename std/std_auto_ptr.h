#ifndef _STD_AUTO_PTR_H
#define _STD_AUTO_PTR_H

#include "std_container.h"

template <class T>
class std_auto_ptr : public std_container<T>{
public:
	std_auto_ptr(T *t = nullptr, bool del = true)
		: std_container<T>(t), _del(del){};

	virtual ~std_auto_ptr(){
		if (_del)
			reset();
	};

	// ==============================

	inline void release(){
		_del = false;
	};

	inline void reset(){
		if (this->ptr()){
			delete this->ptr();
		}
	};

private:
	bool _del;
};

#endif

