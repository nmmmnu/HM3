#ifndef _ICOUNTABLE_H
#define _ICOUNTABLE_H

class ICountable{
public:
	virtual ~ICountable(){};

	virtual uint64_t getCount() const = 0;

	inline bool isEmpty() const;
};

// ==============================

inline bool ICountable::isEmpty() const{
	return getCount() == 0;
}

#endif

