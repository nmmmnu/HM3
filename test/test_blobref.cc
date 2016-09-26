#include "blobref.h"

#include <cstdint>
#include <cstring>

#include <assert.h>
#include "endian.h"

#include <iostream>
#include <iomanip>

int test_blobref(){
	constexpr size_t SIZE = 256;
	char mem[SIZE];

	for(size_t i = 0; i < SIZE; ++i)
		mem[i] = (char) i;

	BlobRef br{ mem };

	assert(*br.as<uint16_t>(0x00) == htobe16(0x0001)	);
	assert(*br.as<uint16_t>(0x0E) == htobe16(0x0E0F)	);
	assert(*br.as<uint32_t>(0x10) == htobe32(0x10111213)	);

	{
		const char *s = br.as<char>('a', 5);
		assert(strncmp(s, "abcde", 5) == 0);

		// relative
		assert(*br.as<char>(s + 5) == 'f');
	}

	{
		struct TestStruct{
			uint16_t	i;
			char		c;
			char		s[4];
		}__attribute__((__packed__));

		const TestStruct *st = br.as<TestStruct>(0x50);
		assert(st->i    == htobe16(0x5051)	);
		assert(st->c    == 0x52			);
		assert(st->s[0] == 0x53			);
		assert(st->s[1] == 0x54			);
		assert(st->s[2] == 0x55			);
		assert(st->s[3] == 0x56			);

		// relative
		const char *stc = (const char *) st;
		assert(*br.as<char>(stc + sizeof(TestStruct)) == 0x57);
	}

	{
		size_t const max = SIZE / sizeof(uint64_t);

		const uint64_t *u64 = br.as<uint64_t>(0, max);
		assert(u64 != nullptr	);
		assert(u64[      0] == htobe64(0x0001020304050607)	);
		assert(u64[max - 1] == htobe64(0xf8f9fafbFCFDFEFF)	);

		// relative
		const uint64_t *p = & u64[max - 2];
		assert(*br.as<uint64_t>(p + 1) == u64[max - 1]	);
	}

	{
		size_t const max = SIZE / sizeof(uint64_t) ;

		// after last
		assert(br.as<uint64_t>(0, max + 1 ) == nullptr		);
	}

	{
		size_t const max = SIZE / sizeof(uint64_t) ;

		const uint64_t *u64 = br.as<uint64_t>(0, max);

		// relative
		// after last
		const uint64_t *p = & u64[max - 1];
		assert(br.as<uint64_t>(p + 1) == nullptr	);
	}

	// zero size
	assert(br.as<char>(0x00, 0) == nullptr	);

	// nullptr
	assert(br.as<char>(nullptr) == nullptr	);

	return 0;
}

int main(){
	test_blobref();
}

