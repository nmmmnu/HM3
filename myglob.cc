#include "myglob.h"

#include <sys/stat.h>	// stat

bool MyGlob::open(const StringRef &path){
	if (_isOpen)
		close();
		
	
	_data.clear();

	if (__open(path.data(), _globresults) == false)	
		return false;

	_isOpen = true;
	
	
	
	size_t i;
	for(i = 0; i < _globresults.gl_pathc; ++i){
		const char *filename = _globresults.gl_pathv[i];

		if (__checkFile( filename ))
			_data.push_back(filename);
	}

	
	
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

void MyGlob::close(){
	if (_isOpen){
		globfree(& _globresults);
		
		_data.clear();
	}
	
	_isOpen = false;
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

