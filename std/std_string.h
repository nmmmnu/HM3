#ifndef _STD_STRING_H
#define _STD_STRING_H

#include <stdlib.h>

class std_string{
private:
	constexpr static const char *EMPTY = "";

public:
	std_string(const char *s): _s(s ? s : EMPTY){};
	std_string(const std_string &s): std_string(s.c_str()){};

	inline const char* c_str() const{
		return _s;
	};

	inline size_t length(const char *s) const{
		return strlen(_s);
	};

	inline int cmp(const char *s) const{
		return strcmp(_s, s);
	};

	inline int cmp(const std_string &s) const{
		return cmp(s.c_str());
	};

	inline char operator[](size_t index) const{
		return index < length() ? _s[index] : '\0';
	}

	inline char operator==(const char *s) const{
		return cmp(s) == 0;
	}

	inline char operator==(const std_string &s) const{
		return _s == s.c_str();
	}

private:
	const char *_s = nullptr;
};

#endif

