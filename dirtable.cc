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

const Pair *DirTable::get(const char *key) const{
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable & file = _files[i];

		const Pair *pair = file.get(key);

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

const Pair *DirTable::_next(){
	const Pair *pair = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable &file = _files[i];

		const Pair *pair2 = file.current();

		// skip if is null
		if (pair2 == nullptr)
			continue;
		// initialize
		if (pair == nullptr){
			pair = pair2;
			continue;
		}

		// compare and swap pair
		if (pair->cmp2(*pair2) > 0)
			pair = pair2;
	}

	if (pair == nullptr)
		return pair;

	// step 2: increase all duplicates
	for(size_t i = 0; i < _filesCount; ++i){
		DiskTable &file = _files[i];

		const Pair *pair2 = file.current();

		if (pair2 && pair->cmp2(*pair2) == 0)
			file.next();
	}

	return pair;
}

