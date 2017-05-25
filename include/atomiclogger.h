#include "logger.h"

#include <mutex>

#ifdef logAtomic__
#undef logAtomic__
#endif

#ifndef MY_ATOMIC_LOGGER_H_
#define MY_ATOMIC_LOGGER_H_

namespace{
	std::mutex g_atomic_logger;

	template<typename T, typename... ARGS>
	void logAtomic__(T first, ARGS... args){
		lock_guard<std::mutex> lock(g_atomic_logger);

		log__(args...);
	}
}

#endif

