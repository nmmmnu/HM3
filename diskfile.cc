#include "diskfile.h"

bool DiskFile::create(const char *filename, IIterator &it, uint64_t datacount){
	if (datacount == 0){
		// very slow operation
		datacount = it.iteratorCount();
	}

	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (! file)
		return false;

	return _writeIteratorToFile(it, datacount, file);
}

bool DiskFile::_writeIteratorToFile(IIterator &it, uint64_t datacount, std::ofstream &file){
	uint64_t be;

	const size_t headerSize = DiskFile::sizeofHeader();
	const size_t tableSize  = sizeof(uint64_t) * datacount;

	/* preallocating the file do not really speed up the fwrite process.
	posix_fallocate(fileno(F), 0, total_size);
	*/

	size_t current = headerSize + tableSize;

	// write table header
	DiskTableHeader header;
	header.size = htobe64(datacount);
	file.write( (const char *) & header, sizeofHeader());

	// traverse and write the table.
	for(auto pair = it.first(); pair; pair = it.next()){
		be = htobe64(current);
		file.write( (const char *) & be, sizeof(uint64_t));
		current += pair.getSize();
	}

	// traverse and write the data.
	for(auto pair = it.first(); pair; pair = it.next()){
		pair.fwrite(file);
	}

	return true;
}

