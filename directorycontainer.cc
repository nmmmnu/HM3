#include "directorycontainer.h"
#include "myglob.h"

bool DirectoryContainer::open(const StringRef &path){
	MyGlob gl;
	if (gl.open(path) == false)
		return false;

	auto vector = gl.getData();

	clear( vector.size() );

	auto it_rend = vector.rend();
	for (auto it = vector.rbegin(); it != it_rend; ++it){
		pushBack(*it);
	}

	return true;
}


