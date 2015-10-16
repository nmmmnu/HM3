#include "multitabledirectory.h"
#include "myglob.h"

bool MultiTableDirectory::open(const StringRef &path){
	_files.clear();

	MyGlob gl;
	if (gl.open(path) == false)
		return false;

	auto vector = gl.getData();

	_files.reserve( vector.size() );

	auto it_rend = vector.rend();
	for (auto it = vector.rbegin(); it != it_rend; ++it){
		DiskTable dt;
		dt.open( *it );
		_files.push_back( std::move(dt) );
	}

	return true;
}


