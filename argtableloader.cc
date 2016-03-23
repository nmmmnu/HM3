#include "argtableloader.h"


namespace hm3{
namespace tableloader{


bool ArgTableLoader::_refresh(){
	_vector.clear();

	if (_argc < 0)
		return true;

	_vector.reserve(_argc);

	for (int i = 0; i < _argc; ++i){
		const char *filename = _argv[i];

		DiskTable dt;
		dt.open(filename);
		_vector.push_back( std::move(dt) );
	}

	return true;
}


} // namespace tableloader
} // namespace

