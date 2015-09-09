#ifndef _MYGLOB_H
#define _MYGLOB_H

#include "icountable.h"

#include <glob.h>	// glob

#include <vector>
#include <string>

class MyGlob{
public:
	MyGlob() = default;
	MyGlob& operator=(MyGlob other) = delete;

	bool open(const std::string &path);
	std::vector<std::string> &getData();
	
private:
	static bool __open(const char *path, glob_t &globresults);
	static void __close(glob_t &globresults);
	static bool __checkFile(const char *filename);

private:
	std::vector<std::string> _data;
};

// ==============================

inline std::vector<std::string> &MyGlob::getData(){
	return _data;
}

#endif

