#include "diskmanager.h"

struct DiskManagerHeader{
	uint64_t	size;
} __attribute__((__packed__));



DiskManager::DiskManager(const char *filename, bool autoOpen){
	_filename = filename;

	if (autoOpen)
		open();
}

DiskManager::~DiskManager(){
	if (_isOpen)
		close();
}

bool DiskManager::open(){
	if (_isOpen)
		return true;

	_isOpen = true;

	return true;
}

void DiskManager::close(){
	if (! _isOpen)
		return;

	_isOpen = false;
}

bool DiskManager::createFromIterator(IIterator *it, uint64_t datacount) const{
	if (datacount == 0 || it == NULL)
		return false;

	FILE *F = fopen(_filename, "w");

	if (F == NULL)
		return false;

	bool result = _writeIteratorToFile(it, datacount, F);

	fclose(F);

	return result;
}

bool DiskManager::_writeIteratorToFile(IIterator *it, uint64_t datacount, FILE *F) const{
	const Pair *pair;
	uint64_t be;

	const size_t headerSize = sizeof(DiskManagerHeader);
	const size_t tableSize  = sizeof(uint64_t) * datacount;

	/* preallocating the file do not really speed up the fwrite process.
	posix_fallocate(fileno(F), 0, total_size);
	*/

	size_t current = headerSize + tableSize;

	// write table header
	DiskManagerHeader header;
	header.size = htobe64(datacount);
	fwrite(& header, sizeof(header), 1, F);

	// traverse and write the table.
	for(pair = it->first(); pair; pair = it->next()){
		be = htobe64(current);
		fwrite(& be, sizeof(uint64_t), 1, F);
		current += pair->getSize();
	}

	// traverse and write the data.
	for(pair = it->first(); pair; pair = it->next()){
		pair->writeToFile(F);
	}

	return true;
}

