#include "listdircollection.h"
#include "myglob.h"

const IROList & ListDirCollection::_getAt(uint64_t index) const{
	return _files[index];
}

uint64_t ListDirCollection::_getCount() const{
	return _files.size();
}

// ==============================

bool ListDirCollection::open(const char *path){
	close();

	MyGlob gl;
	gl.open(path);

	for(const char *filename = gl.first(); filename; filename = gl.next()){
		DiskTable dt;
		dt.open(filename);

		_files.push_front(std::move(dt));
	}

	return true;
}

void ListDirCollection::close(){
	_files.clear();
}

