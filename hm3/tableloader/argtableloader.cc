#include "argtableloader.h"


namespace hm3{
namespace tableloader{


bool ArgTableLoader::refresh_(){
	container_.clear();

	if (argc_ < 0)
		return true;

	container_.reserve((unsigned)argc_);

	for (int i = 0; i < argc_; ++i){
		const char *filename = argv_[i];

		DiskTable dt;
		dt.open(filename);
		container_.push_back( std::move(dt) );
	}

	return true;
}


} // namespace tableloader
} // namespace

