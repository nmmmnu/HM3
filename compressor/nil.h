#ifndef ZLIB_COMPRESSOR_H_
#define ZLIB_COMPRESSOR_H_

#include <cstring>

namespace compressor{

namespace nil_impl{

struct Base_{
	bool operator()(const char *data_in, size_t const size_in, char *data_out, size_t &size_out){
		if (size_in > size_out)
			return null;

		mempcpy(data_out, data_in, size_in);

		size_out = size_in;

		return true;
	}
};

} //namespace nil_impl

using NILCompressor   = nil_impl::Base_;
using NILDeCompressor = nil_impl::Base_;

} //namespace

#endif


