template <class LIST>
bool DiskFile::create(const LIST &list, const StringRef &filename){
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (! file)
		return false;

	return _writeListToFile(list, file);
}

template <class LIST>
bool DiskFile::_writeListToFile(const LIST &list, std::ofstream &file){
	uint64_t be;

	auto const datacount = list.getCount();

	size_t const headerSize = DiskFile::sizeofHeader();
	size_t const tableSize  = sizeof(uint64_t) * datacount;

	/* preallocating the file do not really speed up the fwrite process.
	posix_fallocate(fileno(F), 0, total_size);
	*/

	size_t current = headerSize + tableSize;

	// write table header
	DiskTableHeader header;
	header.size = htobe64(datacount);
	memset(header.padding, 0, DISK_TABLE_PADDING);

	file.write( (const char *) & header, sizeofHeader());

	// traverse and write the table.
	for(auto pair : list){
		be = htobe64(current);
		file.write( (const char *) & be, sizeof(uint64_t));
		current += pair.getSize();
	}

	// traverse and write the data.
	for(auto pair : list){
		pair.fwrite(file);
	}

	return true;
}

