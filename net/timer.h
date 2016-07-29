#ifndef _TIMER_H
#define _TIMER_H

#include "iobuffer.h"

#include <cstdlib>

namespace net{


class Timer_{
private:
	uint32_t	time_	= now_();

protected:
	Timer_() = default;

public:
	bool expired(uint32_t timeout) const{
		return time_ + timeout < now_();
	}

	void restartTimer() {
		time_ = now_();
	}

private:
	static uint32_t now_();
};


} // namespace

#endif

