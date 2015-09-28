#ifndef _ICOUNTABLE_H
#define _ICOUNTABLE_H

#include <cstdint>

template <typename T, typename TCOUNT = uint64_t>
	class ICountable{

public:
	TCOUNT getCount() const{
		return _getCount();
	}

	bool isEmpty() const{
		return _getCount() == 0;
	}

private:
	TCOUNT _getCount() const{
		return impl()._getCount();
	}

private:
	const T &impl() const{
		return *( static_cast<const T*>(this) );
	}
};

#endif

