#include "myglob.h"

#include <glob.h>	// glob
#include <sys/stat.h>	// stat

bool MyGlob::glob(const char *path, std::vector<std::string> & vector){
	vector.clear();

	glob_t globresults;

	int result = ::glob(path, 0, NULL, & globresults);

	if (result == GLOB_NOMATCH)
		return false;

	if (result != 0)
		return false;

	for (size_t i = 0; i < globresults.gl_pathc; ++i){
		const char *filename = globresults.gl_pathv[i];

		if (__checkFile( filename ))
			vector.push_back(filename);
	}

	// close
	globfree(& globresults);

	return vector.size() > 0;
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

