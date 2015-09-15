#ifndef _MYGLOB_H
#define _MYGLOB_H

#include "stringref.h"

#include <glob.h>	// glob

#include <vector>

class MyGlob final{
public:
	MyGlob() = default;
	MyGlob& operator=(MyGlob other) = delete;
	
	~MyGlob(){
		close();
	}

	bool open(const StringRef &path);
	void close();

	std::vector<StringRef> &getData(){
		return _data;
	}
	
private:
	static bool __open(const char *path, glob_t &globresults);
	static bool __checkFile(const char *filename);

private:
	glob_t			_globresults;
	bool			_isOpen		= false;
	std::vector<StringRef>	_data;
};

#endif

