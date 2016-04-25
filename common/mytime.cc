#include "mytime.h"

#include <chrono>

#include <time.h>	// localtime, strftime

char MyTime::buffer[STRING_SIZE];

#if 0
uint64_t MyTime::now() noexcept{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return combine((uint32_t) tv.tv_sec, (uint32_t) tv.tv_usec);
}
#endif

uint64_t MyTime::now() noexcept{
	const auto now = std::chrono::system_clock::now().time_since_epoch();

	const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now);
//	const auto mil = std::chrono::duration_cast<std::chrono::milliseconds>(now - sec);
	const auto mil = std::chrono::duration_cast<std::chrono::microseconds>(now - sec);

	const auto sec_int = (uint32_t) sec.count();
	const auto mil_int = (uint32_t) mil.count();

	return combine(sec_int, mil_int);
}

const char *MyTime::toString(uint64_t date2, const char *format) noexcept{
	time_t date = uncombine(date2);

	if (date == 0)
		return NULL;

	struct tm *tm = localtime(& date);

	if (tm == NULL)
		return NULL;

	strftime(buffer, STRING_SIZE, format, tm);

	return buffer;
}

