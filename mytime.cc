#include "mytime.h"

#include <sys/time.h>
#include <time.h>	// localtime, strftime

char MyTime::buffer[STRING_SIZE];

uint64_t MyTime::now(){
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return combine(tv.tv_sec, tv.tv_usec);
}

const char *MyTime::toString(uint64_t date2, const char *format){
	time_t date = uncombine(date2);

	if (date == 0)
		return NULL;

	struct tm *tm = localtime(& date);

	if (tm == NULL)
		return NULL;

	strftime(buffer, STRING_SIZE, format, tm);

	return buffer;
}

