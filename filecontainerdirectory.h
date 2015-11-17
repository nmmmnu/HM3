#ifndef _FILE_CONTAINER_DIRECTORY_H
#define _FILE_CONTAINER_DIRECTORY_H

#include "myglob.h"

namespace FileListContainer{

template <class CONTAINER>
CONTAINER &loadFromDirectory(CONTAINER &vector, const StringRef &path){
	vector.clear();

	MyGlob gl;
	if (gl.open(path) == false)
		return vector;

	const auto &files = gl.getData();

	vector.reserve(files.size());

	auto it_rend = files.rend();
	for (auto it = files.rbegin(); it != it_rend; ++it){
		const auto &filename = DiskFile::filenameStrip(*it);

                DiskTable dt;
                dt.open(filename);
                vector.push_back( std::move(dt) );
	}

	return vector;
}

}; // namespace ListContainer

#endif

