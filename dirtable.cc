#include "dirtable.h"

#include "myglob.h"

bool DirTable::open(const char *path){
	close();

	MyGlob gl;
	gl.open(path);

	size_t count = 0;
	for(const char *filename = gl.first(); filename; filename = gl.next())
		++count;

	_files = new DiskTable[count];
	_filesCount = count;

	for(const char *filename = gl.first(); filename; filename = gl.next()){
		--count;
		DiskTable & dt = _files[count];
		dt.open(filename);
	}

	return true;
}

void DirTable::close(){
	if (_files == nullptr)
		return;

	delete[] _files;

	_files = nullptr;
	_filesCount = 0;
}

Pair DirTable::_get(const char *key) const{
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable & file = _files[i];

		Pair pair = file.get(key);

		if (pair)
			return pair;
	}

	return nullptr;
}

void DirTable::_rewind(const char *key){
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable &file = _files[i];
		file.first(key);
	}
}

const void *DirTable::_next(){
	Pair         r_pair = nullptr;
//	const void * r_data = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable &file = _files[i];

		Pair pair = file.current();

		// skip if is null
		if (! pair)
			continue;

		// initialize
		if (! r_pair){
			r_pair = pair;
			continue;
		}

		// compare and swap pair
		if (pair.cmp(r_pair) < 0){
			r_pair = pair;
		}
	}

	if (!r_pair)
		return nullptr;

	// step 2: increase all duplicates
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable &file = _files[i];

		Pair pair = file.current();

		if (pair && pair.cmp(r_pair) == 0)
			file.next();
	}
		
	return r_pair.getBlob();
}

