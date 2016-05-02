#ifndef _MYGLOB_H
#define _MYGLOB_H

#include "stringref.h"

#include <glob.h>	// glob

#include <vector>

class MyGlob final{
public:
	MyGlob() = default;

	MyGlob(const MyGlob &other) = delete;
	MyGlob& operator=(MyGlob other) = delete;

	MyGlob(MyGlob &&other);
	MyGlob& operator=(MyGlob &&other);

	void swap(MyGlob &other);

	~MyGlob() noexcept{
		close();
	}

	bool open(const StringRef &path) noexcept;
	void close() noexcept;

	std::vector<StringRef> &getData() noexcept{
		return data_;
	}

private:
	static bool open_(const char *path, glob_t &globresults) noexcept;
	static bool checkFile_(const char *filename) noexcept;

private:
	glob_t			globresults_;
	bool			isOpen_		= false;
	std::vector<StringRef>	data_;
};

#endif

