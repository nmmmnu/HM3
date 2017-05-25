#include "trim.h"

#include <cstring>

constexpr const char *TRIM_CHARACTERS = " \t\r\n";

std::string &trim(std::string &line){
	line.erase(line.find_last_not_of(TRIM_CHARACTERS) + 1);

	return line;
}

inline bool isspace__(char const c){
	static_assert(__builtin_strlen(TRIM_CHARACTERS) == 4, "TRIM_CHARACTERS size changed");

	return	c == TRIM_CHARACTERS[0] ||
		c == TRIM_CHARACTERS[1] ||
		c == TRIM_CHARACTERS[2] ||
		c == TRIM_CHARACTERS[3];
}

char *trim(char *s){
	char *end = s + strlen(s) - 1;
	while(end >= s && isspace__(*end))
		--end;

	*(end + 1) = 0;

	return s;
}

