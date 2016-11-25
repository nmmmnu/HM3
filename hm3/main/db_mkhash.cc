#include "disktable.h"
#include "djbhash.h"
#include "mmapfile.h"

#include "endian.h"	// htobe16

#include <unistd.h>	// access

#include <iostream>

constexpr bool CHECK_FILE_EXISTS = false;

constexpr size_t GROW_FACTOR = 2;

static void printUsage(const char *name);
inline bool fileExists(const StringRef& name);
inline void printStats(size_t size, size_t collisions);

int main(int argc, char **argv){
	if (argc <= 1 + 1){
		printUsage(argv[0]);
		return 1;
	}

	const char *input_file	= argv[1];
	const char *output_file	= argv[2];

	if (CHECK_FILE_EXISTS && fileExists(output_file)){
		std::cout << "File " << output_file << " exists. Please remove it and try again." << '\n';
		return 2;
	}

	using DiskTable = hm3::DiskTable;

	DiskTable list;
	list.open(input_file);

	auto const size = list.getCount() * GROW_FACTOR;
	auto const sizebin = size * sizeof(uint64_t);

	if ( ! MMAPFile::createFile(output_file, sizebin) ){
		std::cout << "Error creating" << output_file << "." << '\n';
		return 1;
	}

	MMAPFile mmap{ MADV_RANDOM };
	mmap.openRW(output_file);

	uint64_t *hashtable = (uint64_t *) mmap.mem();

	// Wow :)
	// memset is not good, because it needs size to be int
	std::cout << "Set hashtable to zeroes." << '\n';
	std::fill(hashtable, hashtable + size, 0);

	size_t collisions = 0;

	std::cout << "Creating hashtable." << '\n';
	DiskTable::size_type index = 0;
	for(const auto &p : list){
		const char *s = p.getKey().data();

		auto const hash = DJBHash(s);

		auto bucket = hash % size;

		if (hashtable[bucket]){
			hashtable[bucket] = 0;
			++collisions;
		}else{
			hashtable[bucket] = htobe64(index);
		}

		++index;
	}

	printStats(size, collisions);
}



inline bool fileExists(const StringRef& name) {
	return access(name.data(), F_OK) != -1;
}

inline void printStats(size_t const size, size_t const collisions){
	constexpr const char *MASKZ = "%-12s : %12zu\n";
	constexpr const char *MASKF = "%-12s : %15.2f %%\n";

	printf(MASKZ, "Total",		size				);
	printf(MASKZ, "Collisions",	collisions			);
	printf(MASKZ, "Diff",		size - collisions		);
	printf(MASKF, "Percent",	100.0 * collisions / size 	);
}


static void printUsage(const char *name){
	std::cout
		<< "Usage:"						<< '\n'
		<< name << " [file.db] [hash_file]"			<< '\n'
		<< "\t\tPath names must be written without extention"	<< '\n'
		<< "\t\tExample 'file.db'"				<< '\n'
		<< "\t\tDo not forget that usually btree_file must have same name as [file.db]"	<< '\n'
	;
}


