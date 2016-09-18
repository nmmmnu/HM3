#ifndef ZLIB_COMPRESSOR_H_
#define ZLIB_COMPRESSOR_H_

#include <cstddef>

namespace compressor{

namespace zlib_impl{

struct Base_{
	// at the time of writting,
	// sizeof(z_stream) = 112
	constexpr static size_t BUFFER_SIZE = 128;

	using buffer_type = char[BUFFER_SIZE];
};

} // namespace zlib_impl

class ZLibCompressor : public zlib_impl::Base_{
public:
	const static int NO_COMPRESSION		;
	const static int BEST_SPEED		;
	const static int BEST_COMPRESSION	;
	const static int DEFAULT_COMPRESSION	;

public:
	ZLibCompressor(int compressionLevel = DEFAULT_COMPRESSION);
	~ZLibCompressor();

	bool operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out);

private:
	buffer_type buffer_;
};

// ===========================

class ZLibDeCompressor : public zlib_impl::Base_{
public:
	ZLibDeCompressor();
	~ZLibDeCompressor();

	bool operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out);

private:
	buffer_type buffer_;
};

} // namespace

#endif


