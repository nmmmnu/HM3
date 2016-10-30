#ifndef FILE_READER_H_
#define FILE_READER_H_

#include <fstream>

#include "stringref.h"

class FileReader{
public:
	FileReader(const StringRef &filename, bool const trim = true) : file_(filename), trim_(trim){}

	std::string getLine() {
		std::string line;

		while( getline(file_, line) ){
			if (trim_)
				trim__(line);

			if (! line.empty() )
				return line;
		}

		// return empty line...
		return line;
	}

	operator bool() const{
		return file_.good();
	}

private:
	static std::string &trim__(std::string &line){
		constexpr const char *trim_ch = " \t\r\n";

		line.erase(line.find_last_not_of(trim_ch) + 1);

		return line;
	}

private:
	std::ifstream	file_;
	bool		trim_;
};

#endif

