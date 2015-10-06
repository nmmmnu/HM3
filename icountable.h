#ifndef _ICOUNTABLE_H
#define _ICOUNTABLE_H

#include <cstdint>

class ICountable{
protected:
	~ICountable() = default;

public:
	typedef uint64_t count_type;

public:
	count_type getCount() const;
	bool isEmpty() const;

private:
	virtual count_type _getCount() const = 0;
};

// ==============================

inline ICountable::count_type ICountable::getCount() const{
	return _getCount();
}

inline bool ICountable::isEmpty() const{
	return _getCount() == 0;
}

#endif

