#ifndef _FILE_CONTAINER_ARGV_H
#define _FILE_CONTAINER_ARGV_H

template <class CONTAINER, class STR_ARRAY>
CONTAINER &loadFileContainer(CONTAINER &vector, unsigned const count, const STR_ARRAY argv){
	vector.clear();

	if (count < 0)
		return vector;

	vector.reserve(count);

	for (unsigned i = 0; i < count; ++i){
		const char *filename = argv[i];

	//	printf("%5d %s\n", i, count, filename);

                DiskTable dt;
                dt.open(filename);
                vector.push_back( std::move(dt) );
	}

	return vector;
}

template <class CONTAINER>
inline CONTAINER &loadFileContainerFromArgv(CONTAINER &vector, int const argc, const char **argv){
	return loadFileContainer(vector, argc - 1, & argv[1]);
}

#endif

