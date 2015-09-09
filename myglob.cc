#include "myglob.h"

#include <sys/stat.h>	// stat

bool MyGlob::open(const std::string &path){
	glob_t globresults;
	_data.clear();

	if (__open(path.c_str(), globresults) == false)	
		return false;


	
	size_t i;
	for(i = 0; i < globresults.gl_pathc; ++i){
		const char *filename = globresults.gl_pathv[i];

		if (__checkFile( filename ))
			_data.push_back(filename);
	}



	__close(globresults);
	
	return i > 0;
}

bool MyGlob::__open(const char *path, glob_t &globresults){
	int result = glob(path, 0, nullptr, & globresults);

	if (result != 0)
		return false;

	if (result == GLOB_NOMATCH)
		return false;

	return true;
}

void MyGlob::__close(glob_t &globresults){
	globfree(& globresults);
}

bool MyGlob::__checkFile(const char *filename){
	struct stat s;
	stat(filename, & s);

	if (s.st_mode & S_IFREG)	// file
		return true;

	if (s.st_mode & S_IFLNK)	// symlink
		return true;

	return false;
}

