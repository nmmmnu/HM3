#ifndef _ICOUNTABLE_H
#define _ICOUNTABLE_H

#include <stdint.h>

class ICountable{
public:
	virtual ~ICountable(){};
	
	uint64_t getCount() const;
	bool isEmpty() const;

private:
	virtual uint64_t _getCount() const = 0;
};

// ==============================

inline uint64_t ICountable::getCount() const{
	return _getCount();	
}

inline bool ICountable::ICountable::isEmpty() const{
	return _getCount() == 0;
}

#endif

