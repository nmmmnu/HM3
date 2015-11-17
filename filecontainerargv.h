#ifndef _FILE_CONTAINER_ARGV_H
#define _FILE_CONTAINER_ARGV_H

namespace FileListContainer{

template <class CONTAINER, class STR_ARRAY>
CONTAINER &load(CONTAINER &vector, unsigned const count, const STR_ARRAY argv){
	vector.clear();

	if (count < 0)
		return vector;

	vector.reserve(count);

	for (unsigned i = 0; i < count; ++i){
		const auto &filename = DiskFile::filenameStrip( argv[i] );

                DiskTable dt;
                dt.open(filename);
                vector.push_back( std::move(dt) );
	}

	return vector;
}

template <class CONTAINER>
inline CONTAINER &loadFromArgv(CONTAINER &vector, int const argc, const char **argv){
	return load(vector, argc - 1, & argv[1]);
}

}; // namespace ListContainer

#endif

