#ifndef _STD_CONTAINER_H
#define _STD_CONTAINER_H

template <class T>
class std_container{
public:
	std_container(T *t)
		: _t(t){};

	virtual ~std_container(){};

	inline T * ptr() const{
		return _t;
	}

	inline T & value() const{
		return *_t;
	}

	// ==============================

	inline T & operator *() const {
		return value();
	}

//	inline T & operator ()() const {
//		return value();
//	}

	inline T * operator ->() const {
		return ptr();
	}

protected:
	inline void setPtr(T *t){
		_t = t;
	}

private:
	T *_t;
};

#endif

