#include "mytime.h"

#include <sys/time.h>
#include <time.h>	// localtime, strftime
#include <stdio.h>

char MyTime::buffer[STRING_SIZE]; //"1980-01-01 00:00:00";

const char *MyTime::DATA_FORMAT_STANDARD = "%Y-%m-%d %H:%M:%S";
const char *MyTime::DATA_FORMAT_NUMBER = "%Y%m%d.%H%M%S";

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

