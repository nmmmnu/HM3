#include <fstream>

template <class LIST>
bool DiskFile::createFromList(const LIST &list,
			bool const keepInvalid, bool const keepTombstones) const{

	std::ofstream fileMeta(filename_meta,	std::ios::out | std::ios::binary);
	std::ofstream fileIndx(filename_indx,	std::ios::out | std::ios::binary);
	std::ofstream fileData(filename_data,	std::ios::out | std::ios::binary);

	return writeListToFile(list, fileMeta, fileIndx, fileData, keepInvalid, keepTombstones);
}

template <class LIST>
bool DiskFile::writeListToFile(const LIST &list,
			std::ofstream &file_meta,
			std::ofstream &file_indx,
			std::ofstream &file_data,
			bool const keepInvalid, bool const keepTombstones) const{
	uint64_t be;

	size_t current = 0;

	size_t datacount = 0;
	size_t tombstones = 0;
	uint64_t createdMin = 0;
	uint64_t createdMax = 0;

	bool first_created = true;

	bool const tombstoneCheck = ! keepTombstones;

	for(const auto &pair : list){
		if (! pair )
			continue;

		if (keepInvalid == false && pair.valid(tombstoneCheck) == false)
			continue;

		if (pair.isTombstone())
			++tombstones;

		if (first_created){
			uint64_t created = pair.getCreated();

			createdMin = created;
			createdMax = created;

			first_created = false;
		}else{
			uint64_t created = pair.getCreated();

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

		current += pair.getSize();
		++datacount;

	//	pair.print();
	}

	// now we miraculasly have the datacount.

	// write table header
	auto header = DiskFileHeader::create(datacount, tombstones, createdMin, createdMax);

	file_meta.write( (const char *) & header, sizeof(header));

	return true;
}

