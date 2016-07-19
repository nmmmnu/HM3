#ifndef STRING_IMPLODER_H
#define STRING_IMPLODER_H

#include <sstream>

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

#endif

