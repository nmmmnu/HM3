#ifndef ZLIB_EASY_COMPRESSOR_H_
#define ZLIB_EASY_COMPRESSOR_H_

#include <cstddef>

namespace compressor{

struct ZLibEasyCompressor{
	bool operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out);
};

struct ZLibEasyDeCompressor{
	bool operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out);
};

struct ZLibEasyCompressorTag{
	using fw_type	= ZLibEasyCompressor;
	using bw_type	= ZLibEasyDeCompressor;
};

} //namespace

#endif


