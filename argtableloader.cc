#include "argtableloader.h"

auto ArgTableLoader::_load(int const argc, const char **argv) -> container_type{
	container_type vector;

	if (argc < 0)
		return vector;

	vector.reserve(argc);

	for (int i = 0; i < argc; ++i){
		const char *filename = argv[i];

		DiskTable dt;
		dt.open(filename);
		vector.push_back( std::move(dt) );
	}

	return vector;
}

