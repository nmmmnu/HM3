#if 0
#include "zlibeasycompressor.h"

using   Compressor = compressor::ZLibEasyCompressor;
using DeCompressor = compressor::ZLibEasyDeCompressor;

#else

#include "zlibcompressor.h"

using   Compressor = compressor::ZLibCompressor;
using DeCompressor = compressor::ZLibDeCompressor;

#endif

#include <cstring>

#include <iostream>
#include <iomanip>

#include <cstdio>

constexpr size_t SIZE = 1024;

int main(){
	Compressor     compressor;
	DeCompressor decompressor;

	const char *odata = "Hello World";
	size_t osize = strlen(odata);

	char cbuffer[SIZE];
	size_t csize = SIZE;

	char dbuffer[SIZE];
	size_t dsize = SIZE;

	bool cr;
	bool dr;

		cr =   compressor(odata,   osize, cbuffer, csize);

	for(size_t i = 0; i < 1000000; ++i){
		dr = decompressor(cbuffer, csize, dbuffer, dsize);
	}

	std::cout << std::fixed << std::setprecision(2) << ( float(osize) / float(csize) )
			<< " % compressed"
			<< '\n';

	if ( ! cr || ! dr || osize != dsize || memcmp(odata, dbuffer, osize) ){
		std::cout << "Error" << '\n';
	}
}

