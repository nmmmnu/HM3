#ifndef _MYGLOB_H
#define _MYGLOB_H

#include <vector>
#include <string>

class MyGlob{
public:
	static bool glob(const char *path, std::vector<std::string> & vector);

private:
	static bool __checkFile(const char *filename);

};

#endif

