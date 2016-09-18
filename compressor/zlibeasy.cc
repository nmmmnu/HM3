#include "zlibeasy.h"

#include <cstring>

#include <zlib.h>

namespace compressor{

bool ZLibEasyCompressor::operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out){
	if (data_in == nullptr || size_in == 0 || data_out == nullptr || size_out == 0)
		return false;

	z_stream zs;
	memset(&zs, 0, sizeof(z_stream));
	if ( deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK )
		return false;

	zs.next_in   = reinterpret_cast<Bytef *>( const_cast<char *>( data_in ) );
	zs.avail_in  = static_cast<uInt>( size_in );

	zs.next_out  = reinterpret_cast<Bytef *>( data_out );
	zs.avail_out = static_cast<uInt>( size_out );

	int const result = deflate(&zs, Z_FINISH);

	size_out = zs.total_out;

	deflateEnd(&zs);

	return result == Z_STREAM_END;
}

// ===========================

bool ZLibEasyDeCompressor::operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out){
	if (data_in == nullptr || size_in == 0 || data_out == nullptr || size_out == 0)
		return false;

	z_stream zs;
	memset(&zs, 0, sizeof(z_stream));
	if ( inflateInit(&zs) != Z_OK )
		return false;

	zs.next_in   = reinterpret_cast<Bytef *>( const_cast<char *>( data_in ) );
	zs.avail_in  = static_cast<uInt>( size_in );

	zs.next_out  = reinterpret_cast<Bytef *>( data_out );
	zs.avail_out = static_cast<uInt>( size_out );

	int const result = inflate(&zs, Z_FINISH);

	size_out = zs.total_out;

	inflateEnd(&zs);

	return result == Z_STREAM_END;
}

} //namespace

