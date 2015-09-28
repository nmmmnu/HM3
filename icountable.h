#ifndef _ICOUNTABLE_H
#define _ICOUNTABLE_H

#include <cstdint>

template <typename T, typename TCOUNT = uint64_t>
class ICountable{
public:
	typedef TCOUNT count_type;

public:
	count_type getCount() const{
		return impl()._getCount();
	}

	bool isEmpty() const{
		return impl()._getCount() == 0;
	}

private:
	const T &impl() const{
		return *( static_cast<const T*>(this) );
	}
};

#endif

