#include "blobref.h"

#include <cstdint>
#include <cstring>

#include <assert.h>
#include <endian.h>

int test_blobref(){
	constexpr size_t SIZE = 256;
	char mem[SIZE];

	for(int i = 0; i < SIZE; ++i)
		mem[i] = (char) i;

	BlobRef br{ mem };

	assert(*br.as<uint16_t>(0x00) == htobe16(0x0001)	);
	assert(*br.as<uint16_t>(0x0E) == htobe16(0x0E0F)	);
	assert(*br.as<uint32_t>(0x10) == htobe32(0x10111213)	);

	{
		const char *s = br.as<char>('a');
		assert(strncmp(s, "abcde", 5) == 0);
	}

	{
		struct TestStruct{
			uint16_t	i;
			char		c;
			char		s[4];
		}__attribute__((__packed__));

		const TestStruct *st = br.as<TestStruct>(0x50);
		assert(st->i == htobe16(0x5051)		);
		assert(st->c == 0x52			);

		const char *s1 = st->s;
		const char s2[] = { 0x53, 0x54, 0x55, 0x56, 0x57 };
		assert(strncmp(s1, s2, sizeof s2) == 0);
	}

	{
		size_t const pos = SIZE / sizeof(uint64_t);

		const uint64_t *u64 = br.as<uint64_t>(0, pos);
		assert(u64 != nullptr	);
		assert(u64[      0] == htobe64(0x0001020304050607)	);
		assert(u64[pos - 1] == htobe64(0xf8f9fafbFCFDFEFF)	);
	}

	{
		size_t const pos = SIZE / sizeof(uint64_t) ;

		const uint64_t *u64 = br.as<uint64_t>(0, pos + 1 );
		assert(u64 == nullptr	);
	}

	return 0;
}

int main(){
	test_blobref();
}

