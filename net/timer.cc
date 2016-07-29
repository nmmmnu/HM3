#include "timer.h"

#include <chrono>

namespace net{


uint32_t Timer_::now_(){
	const auto now = std::chrono::system_clock::now().time_since_epoch();

	const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now);

	return (uint32_t) sec.count();
}


} // namespace

