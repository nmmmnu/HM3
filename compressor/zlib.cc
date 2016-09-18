#include "zlib.h"

#include <stdexcept>

#include <cstring>

#include <zlib.h>

namespace compressor{

namespace zlib_impl{

template<bool OP>
struct OpsBase_;

template<>
struct OpsBase_<true>{
	static bool init(z_stream *zs, int const compressionLevel){
		return deflateInit(zs, compressionLevel) == Z_OK;
	}

	static int done(z_stream *zs){
		return deflateEnd(zs);
	}

	static int reset(z_stream *zs){
		return deflateReset(zs);
	}

	static int process(z_stream *zs){
		return deflate(zs, Z_FINISH);
	}
};

// ===========================

template<>
struct OpsBase_<false>{
	static bool init(z_stream *zs, int const){
		return inflateInit(zs) == Z_OK;
	}

	static int done(z_stream *zs){
		return inflateEnd(zs);
	}

	static int reset(z_stream *zs){
		return inflateReset(zs);
	}

	static int process(z_stream *zs){
		return inflate(zs, Z_FINISH);
	}
};

// ===========================

template<bool OP>
struct Ops_{
	static void init(z_stream *zs, int const compressionLevel){
		memset(zs, 0, sizeof(z_stream));

		int const result = OpsBase_<OP>::init(zs, compressionLevel);

		if (!result){
			std::bad_alloc exception;
			throw exception;
		}
	}

	static int done(z_stream *zs){
		return OpsBase_<OP>::done(zs);
	}

	static bool process(z_stream *zs, const char *data, size_t const size, char *data_out, size_t &size_out){
		if (data == nullptr || size == 0 || data_out == nullptr || size_out == 0)
			return false;

		OpsBase_<OP>::reset(zs);

		zs->next_in   = reinterpret_cast<Bytef *>( const_cast<char *>( data ) );
		zs->avail_in  = static_cast<uInt>( size );

		zs->next_out  = reinterpret_cast<Bytef *>( data_out );
		zs->avail_out = static_cast<uInt>( size_out );

		int const result = OpsBase_<OP>::process(zs);

		size_out = zs->total_out;

		return result == Z_STREAM_END;
	}
};

} //namespace zlib_impl

// ===========================

namespace {

inline z_stream *cast_(zlib_impl::Base_::buffer_type buffer){
	static_assert( sizeof(z_stream) < sizeof(zlib_impl::Base_::buffer_type), "Increase the buffer");

	return reinterpret_cast<z_stream *>(buffer);
}

} // namespace

// ===========================

const int ZLibCompressor::NO_COMPRESSION	= Z_NO_COMPRESSION	;
const int ZLibCompressor::BEST_SPEED		= Z_BEST_SPEED		;
const int ZLibCompressor::BEST_COMPRESSION	= Z_BEST_COMPRESSION	;
const int ZLibCompressor::DEFAULT_COMPRESSION	= Z_DEFAULT_COMPRESSION	;

ZLibCompressor::ZLibCompressor(int const compressionLevel){
	zlib_impl::Ops_<true>::init(cast_(buffer_), compressionLevel);
}

ZLibCompressor::~ZLibCompressor(){
	zlib_impl::Ops_<true>::done(cast_(buffer_));
}

bool ZLibCompressor::operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out){
	return zlib_impl::Ops_<true>::process(cast_(buffer_), data_in, size_in, data_out, size_out);
}

// ===========================

ZLibDeCompressor::ZLibDeCompressor(){
	zlib_impl::Ops_<false>::init(cast_(buffer_), 0);
}

ZLibDeCompressor::~ZLibDeCompressor(){
	zlib_impl::Ops_<false>::done(cast_(buffer_));
}

bool ZLibDeCompressor::operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out){
	return zlib_impl::Ops_<false>::process(cast_(buffer_), data_in, size_in, data_out, size_out);
}

} // namespace

