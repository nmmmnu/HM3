#include "directorytableloader.h"

#include "myglob.h"

bool DirectoryTableLoader::refresh(){
	_vector.clear();

	MyGlob gl;
	if (gl.open(_path) == false)
		return true;

	const auto &files = gl.getData();

	_vector.reserve(files.size());

	auto it_rend = files.rend();
	for (auto it = files.rbegin(); it != it_rend; ++it){
		const auto &filename = *it;

                DiskTable dt;
                dt.open(filename);
                _vector.push_back( std::move(dt) );
	}

	return true;
}

