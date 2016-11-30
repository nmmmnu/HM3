#include "mmapfile.h"
#include "myhash.h"

#include "hashindexfilenames.h"

#include "logger.h"

namespace hm3{
namespace hashindex{

template <class LIST>
bool HashIndexBuilder::createFromList(const StringRef &filename2, const LIST &list, bool const stats){
	size_t const hashtableSize = list.getCount() * growFactor_;
	size_t const sizeBin       = hashtableSize * sizeof(uint64_t);

	const std::string filename = filenameIndx(filename2);

	// create file

	if ( ! MMAPFile::createFile(filename, sizeBin) ){
		return false;
	}

	// open file

	MMAPFile mmap{ MADV_RANDOM };
	if ( ! mmap.openRW(filename) ){
		return false;
	}

	uint64_t *hashtable = (uint64_t *) mmap.mem();

	// memset is not good, because it needs size to be int
	log__("Set hashtable to zeroes.");
	std::fill(hashtable, hashtable + hashtableSize, htobe64(BUCKET_EMPTY));

	// process

	size_t const collisions = createFromList_(hashtable, hashtableSize, list);

	if (stats)
		printStats_(list.getCount(), collisions);

	return true;
}

template <class LIST>
size_t HashIndexBuilder::createFromList_(uint64_t *hashtable, size_t const hashtableSize, const LIST &list){
	uint64_t const BE_BUCKET_EMPTY		= htobe64(BUCKET_EMPTY		);
	uint64_t const BE_BUCKET_COLLISION	= htobe64(BUCKET_COLLISION	);

	auto CalcHash = DJB2Hash<uint64_t>;

	size_t collisions = 0;

	log__("Creating hashtable.");
	DiskTable::size_type index = 0;
	for(const auto &p : list){
		const char *s = p.getKey().data();

		auto const hash = CalcHash(s);

		auto const bucket = hash % hashtableSize;

		auto const htb = hashtable[bucket];


		if (htb == BE_BUCKET_EMPTY){
			hashtable[bucket] = htobe64(index);

		}else{
			++collisions;

			if (htb != BE_BUCKET_COLLISION){
				hashtable[bucket] = BE_BUCKET_COLLISION;
				++collisions;
			}
		}


		++index;
	}

	return collisions;
}

void HashIndexBuilder::printStats_(size_t const size, size_t const collisions){
	constexpr const char *MASKZ = "%-16s : %12zu\n";
	constexpr const char *MASKF = "%-16s : %15.2f %%\n";

	double const percent = 100.0 * collisions / size;

	printf(MASKZ, "Total",			size			);
	printf(MASKZ, "Total Present",		size - collisions	);
	printf(MASKF, "Percent Present",	100 - percent		);
	printf(MASKZ, "Collisions",		collisions		);
	printf(MASKF, "Percent Missing",	percent			);
}


} // namespace btreeindex
} // namespace

