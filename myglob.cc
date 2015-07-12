#include "myglob.h"

#include <sys/stat.h>	// stat

MyGlob::MyGlob(MyGlob &&other) :
		_isOpen(other._isOpen),
		_globresults(other._globresults),
		_it(other._it){
	other._isOpen = false;
}

bool MyGlob::open(const char *path){
	close();

	int result = glob(path, 0, nullptr, & _globresults);

	switch(result){
	case 0:
		_isOpen = true;
		return true;

	case GLOB_NOMATCH:
		return false;

	default:
		return false;
	}
}

void MyGlob::close(){
	if (! _isOpen)
		return;

	globfree(& _globresults);
	_isOpen = false;
}

const char *MyGlob::next(){
	if (! _isOpen)
		return nullptr;

	while(_it < _globresults.gl_pathc){
		const char *filename = _globresults.gl_pathv[_it++];

		if (__checkFile( filename ))
			return filename;
	}

	return nullptr;
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

