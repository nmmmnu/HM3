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

#if 0

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
		if (!dr){
			std::cout << "Error?" << '\n';
			return 1;
		}
	}

	std::cout << std::fixed << std::setprecision(2) << ( float(osize) / float(csize) )
			<< " % compressed"
			<< '\n';

	if ( ! cr || ! dr || osize != dsize || memcmp(odata, dbuffer, osize) ){
		std::cout << "Error" << '\n';
	}
}
#endif

bool test(Compressor &compressor, DeCompressor &decompressor, const char *data){
	// compressor

	static char cbuffer[SIZE];
	size_t csize = SIZE;

	size_t const size = strlen(data);

	bool const cresult = compressor(data, size, cbuffer, csize);

	if (! cresult)
		std::cout << "  compressor error" << '\n';

	// decompressor

	static char dbuffer[SIZE];
	size_t dsize = SIZE;

	bool const dresult = decompressor(cbuffer, csize, dbuffer, dsize);

	if (! dresult)
		std::cout << "decompressor error" << '\n';

	// summary

	if (size == dsize && memcmp(data, dbuffer, size) == 0){
		std::cout	<< std::fixed << std::setprecision(2) << ( float(size) / float(csize) )
				<< " % compressed"
				<< '\n';
		return true;
	}

	std::cout << "Fail !!!" << '\n';
	return false;

}

int main(){
	Compressor     compressor;
	DeCompressor decompressor;

	test(compressor, decompressor, "Hello World"		);
	test(compressor, decompressor, "My name is Malcolm"	);
	test(compressor, decompressor, "Where are you going?"	);
	test(compressor, decompressor, "How are you today?"	);
}

