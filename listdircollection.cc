#include "listdircollection.h"
#include "myglob.h"

bool ListDirCollection::open(const StringRef &path){
	close();

	MyGlob gl;
	if (gl.open(path) == false)
		return false;

	auto vector = gl.getData();

	_files.reserve( vector.size() );

	for (auto it = vector.rbegin(); it != vector.rend(); ++it){
		DiskTable dt;
		dt.open( *it );
		_files.push_back( std::move(dt) );
	}

	return true;
}

void ListDirCollection::close(){
	_files.clear();
}

