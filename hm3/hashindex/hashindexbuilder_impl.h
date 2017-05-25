#include "mmapfile.h"

#include "hashindexfilenames.h"

#include "logger.h"

namespace hm3{
namespace hashindex{

template <class LIST>
bool HashIndexBuilder::createFromList(const StringRef &filename2, const LIST &list, bool const stats) const{
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
size_t HashIndexBuilder::createFromList_(uint64_t *hashtable, size_t const hashtableSize, const LIST &list) const{
	uint64_t const BE_BUCKET_EMPTY		= htobe64(BUCKET_EMPTY		);
	uint64_t const BE_BUCKET_COLLISION	= htobe64(BUCKET_COLLISION	);

	size_t collisions = 0;

	log__("Creating hashtable.");
	DiskTable::size_type index = 0;
	for(const auto &p : list){
		auto const bucket = bucketForKey_(p.getKey(), hashtableSize);

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

uint64_t HashIndexBuilder::bucketForKey_(const StringRef &key, size_t const hashtableSize) const{
	const char *s = key.data();

	auto const hash = hash_(s);

	return hash % hashtableSize;
}

void HashIndexBuilder::printStats_(size_t const size, size_t const collisions){
	constexpr const char *MASKZ = "%-16s : %12zu\n";
	constexpr const char *MASKF = "%-16s : %15.2f %%\n";

	double const percent = 100.0 * (double) collisions / (double) size;

	printf(MASKZ, "Total",			size			);
	printf(MASKZ, "Total Present",		size - collisions	);
	printf(MASKF, "Percent Present",	100 - percent		);
	printf(MASKZ, "Collisions",		collisions		);
	printf(MASKF, "Percent Missing",	percent			);
}


} // namespace btreeindex
} // namespace

