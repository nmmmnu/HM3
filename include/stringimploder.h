#ifndef STRING_IMPLODER_H
#define STRING_IMPLODER_H

#include <sstream>

#if 0
class StringImploder{
public:
	template<class CONTAINER>
	std::string operator() (const CONTAINER &container, char const delimiter){
		// "fast version" stolen from http://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
		// there is no ss_.str(std::string &&), but this could be still faster
		ss_.str(std::string());

		bool first = true;

		for(const auto &s : container){
			if (first){
				first = false;
			}else{
				ss_.put(delimiter);
			}

			ss_ << s;
		}

		return ss_.str();
	}

private:
	std::stringstream ss_;
};
#else

struct StringImploder{
	template<class CONTAINER>
	std::string operator() (const CONTAINER &container, char const delimiter){
		std::stringstream ss;

		bool first = true;

		for(const auto &s : container){
			if (first){
				first = false;
			}else{
				ss.put(delimiter);
			}

			ss << s;
		}

		return ss.str();
	}
};


#endif


#endif

