
template <class LIST>
bool DiskFile::create(const LIST &list,
			const StringRef &filename_meta,
			const StringRef &filename_indx,
			const StringRef &filename_data,
			bool const keepInvalid, bool const keepTombstones){

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
			bool const keepInvalid, bool const keepTombstones){
	uint64_t be;

	size_t current = 0;
	size_t datacount = 0;

	bool const tombstoneCheck = ! keepTombstones;

	for(const auto &pair : list){
		if (pair.isNULL())
			continue;

		if (keepInvalid == false && pair.valid(tombstoneCheck) == false)
			continue;

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
	DiskTableHeader header;
	header.size   = htobe64(datacount);
	header.sorted = SORTED;

	file_meta.write( (const char *) & header, sizeof(DiskTableHeader));

	return true;
}

