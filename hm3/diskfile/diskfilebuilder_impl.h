#include "diskfileheader.h"

#include <endian.h>

#include <fstream>

namespace hm3{
namespace diskfile{


template <class ITERATOR>
bool DiskFileBuilder::createFromIterator(const StringRef &filename, const ITERATOR &begin, const ITERATOR &end,
			bool keepTombstones) const{

	std::ofstream fileMeta(filenameMeta(filename),	std::ios::out | std::ios::binary);
	std::ofstream fileIndx(filenameIndx(filename),	std::ios::out | std::ios::binary);
	std::ofstream fileData(filenameData(filename),	std::ios::out | std::ios::binary);

	return _writeIteratorToFile(begin, end, fileMeta, fileIndx, fileData, keepTombstones);
}


template <class ITERATOR>
bool DiskFileBuilder::_writeIteratorToFile(const ITERATOR &begin, const ITERATOR &end,
			std::ofstream &file_meta,
			std::ofstream &file_indx,
			std::ofstream &file_data,
			bool const keepTombstones) const{
	uint64_t be;

	size_t current = 0;

	size_t datacount = 0;
	size_t tombstones = 0;
	uint64_t createdMin = 0;
	uint64_t createdMax = 0;

	bool first_created = true;

	for(ITERATOR it = begin; it != end; ++it){
		const auto &pair = *it;

		if (! pair )
			continue;

		if (pair.isTombstone()){
			if (keepTombstones == false)
					continue;

			++tombstones;
		}

		if (first_created){
			uint64_t const created = pair.getCreated();

			createdMin = created;
			createdMax = created;

			first_created = false;
		}else{
			uint64_t const created = pair.getCreated();

			if (created < createdMin)
				createdMin = created;

			if (created > createdMax)
				createdMax = created;
		}

		// write the index
		be = htobe64(current);
		file_indx.write( (const char *) & be, sizeof(uint64_t));

		// write the data
		pair.fwrite(file_data);

		current += pair.getBytes();
		++datacount;

	//	pair.print();
	}

	// now we miraculasly have the datacount.

	// write table header
	DiskFileHeader head{datacount, true, tombstones, createdMin, createdMax};
	return head.writeToFile(file_meta);
}


} // namespace diskfile
} // namespace

