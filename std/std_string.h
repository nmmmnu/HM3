#include <stdio.h>
#include <string.h>

class std_string_ref{
public:
	std_string_ref(const char *s) :
		s(s){};

	inline operator const char *() const{
		return s;
	}

	inline const char *c_str() const{
		return s;
	}

	inline const char *value() const{
		return c_str();
	}

	inline size_t length() const{
		return strlen(s);
	}

	inline char operator[](size_t index) const{
		return index < length() ? s[index] : '\0';
	}

	inline bool operator==(const char *s2) const{
		return cmp(s2);
	}

	inline bool operator==(const std_string_ref &sr) const{
		return cmp(sr.c_str());
	}

private:
	inline bool cmp(const char *s2) const{
		return strcmp(s, s2) == 0;
	};

	const char *s;
};

int main(int argc, char** argv){
	const char *hello = "hello";
	const char *world = "world";

	std_string_ref a = hello;

	a = world;

	std_string_ref b = a;
	printf("%s\n", (const char *)b);


	printf("%d\n", a == b);

	return 0;
}

