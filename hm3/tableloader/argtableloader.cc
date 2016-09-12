#include "argtableloader.h"

#include "arrayref.h"

namespace hm3{
namespace tableloader{


bool ArgTableLoader::refresh_(){
	container_.clear();

	if (argc_ < 0)
		return true;

	container_.reserve(argc_);

	for(const char *filename : ArrayRef<const char *>{ argv_, (size_t) argc_ } ){
		DiskTable dt;
		dt.open(filename);
		container_.push_back( std::move(dt) );
	}

	return true;
}


} // namespace tableloader
} // namespace

