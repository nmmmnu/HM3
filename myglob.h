#ifndef _MYGLOB_H
#define _MYGLOB_H

#include "stringref.h"

#include <glob.h>	// glob

#include <vector>

class MyGlob final{
public:
	MyGlob() = default;
	MyGlob& operator=(MyGlob other) = delete;

	~MyGlob() noexcept{
		close();
	}

	bool open(const StringRef &path) noexcept;
	void close() noexcept;

	std::vector<StringRef> &getData() noexcept{
		return _data;
	}

private:
	static bool __open(const char *path, glob_t &globresults) noexcept;
	static bool __checkFile(const char *filename) noexcept;

private:
	glob_t			_globresults;
	bool			_isOpen		= false;
	std::vector<StringRef>	_data;
};

#endif

