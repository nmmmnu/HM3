#include "directorytableloader.h"

#include "myglob.h"


namespace hm3{
namespace tableloader{


bool DirectoryTableLoader::refresh(){
	container_.clear();

	MyGlob gl;
	if (gl.open(path_) == false)
		return true;

	const auto &files = gl.getData();

	container_.reserve(files.size());

	// order in the iterator, does not matter
	// but order in get, still very important
	auto it_rend = files.rend();
	for (auto it = files.rbegin(); it != it_rend; ++it){
		const auto &filename = *it;

                DiskTable dt;
                dt.open(filename);
                container_.push_back( std::move(dt) );
	}

	return true;
}


} // namespace tableloader
} // namespace

