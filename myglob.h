#ifndef _MYGLOB_H
#define _MYGLOB_H

#include "icountable.h"

#include <stdint.h>
#include <stdlib.h>	// size_t
#include <glob.h>	// glob

class MyGlob{
public:
	MyGlob(){};
	MyGlob(MyGlob &&other);
	MyGlob& operator=(MyGlob other) = delete;
	virtual ~MyGlob(){
		close();
	};

	bool open(const char *path);
	void close();

	inline void rewind();
	inline const char *first();
	const char *next();

private:
	static bool __checkFile(const char *filename);

private:
	bool _isOpen = false;

	glob_t _globresults;

	size_t _it = 0;
};

inline void MyGlob::rewind(){
	_it = 0;
}

inline const char *MyGlob::first(){
	rewind();
	return next();
}

#endif

