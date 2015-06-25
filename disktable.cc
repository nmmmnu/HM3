#include "disktable.h"

#include <stddef.h>	// ofsetof
#include <sys/mman.h>	// mmap

struct DiskTableHeader{
	uint64_t	size;		// 8
	uint64_t	data[1];	// 8, dynamic
} __attribute__((__packed__));

constexpr size_t DiskTable::__sizeofHeader(){
	return offsetof(DiskTableHeader, data);
}

DiskTable::DiskTable(const char *filename){
	_filename = filename;
}

DiskTable::~DiskTable(){
	close();
}

bool DiskTable::open(){
	close();

	FILE *F = fopen(_filename, "r");

	if (F == NULL)
		return false;

	fseek(F, 0, SEEK_END);

	off_t size = ftello(F);

	if (size == 0){
		fclose(F);
		return false;
	}

	const void *mem = mmap(NULL, size, PROT_READ, MAP_SHARED, fileno(F), /* offset */ 0);

	if (mem == MAP_FAILED){
		fclose(F);
		return false;
	}

	_f = F;
	_size = size;
	_mem = mem;

	// memoize count instead to read it from the disk each time
	_datacount = _getCount();

	return true;
}

void DiskTable::close(){
	if (_mem == NULL || _size == 0)
		return;

	munmap((void *) _mem, _size);
	fclose(_f);

	_mem = NULL;
	_size = 0;
}

OPair DiskTable::getAt(uint64_t index) const{
	if (index >= getCount())
		return NULL;

	const char *mem = (char *) _mem;
	const DiskTableHeader *head = (DiskTableHeader *) _mem;

	const uint64_t ptr = be64toh( head->data[index] );
	const Pair *pair = (Pair *) & mem[ptr];

	return OPair(pair);
}

size_t DiskTable::getSize() const{
	return _size - __sizeofHeader() - _datacount * sizeof(uint64_t);
}

uint64_t DiskTable::_getCount() const{
	const DiskTableHeader *head = (DiskTableHeader *) _mem;
	return be64toh(head->size);
}

// ==============================

bool DiskTable::create(const char *filename, IIterator *it, uint64_t datacount){
	if (it == NULL)
		return NULL;

	if (datacount == 0){
		// very slow operation
		datacount = it->iteratorCount();
	}

	FILE *F = fopen(filename, "w");

	if (F == NULL)
		return false;

	bool result = _writeIteratorToFile(it, datacount, F);

	fclose(F);

	return result;
}

bool DiskTable::_writeIteratorToFile(IIterator *it, uint64_t datacount, FILE *F){
	const Pair *pair;
	uint64_t be;

	const size_t headerSize = __sizeofHeader();
	const size_t tableSize  = sizeof(uint64_t) * datacount;

	/* preallocating the file do not really speed up the fwrite process.
	posix_fallocate(fileno(F), 0, total_size);
	*/

	size_t current = headerSize + tableSize;

	// write table header
	DiskTableHeader header;
	header.size = htobe64(datacount);
	fwrite(& header, headerSize, 1, F);

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

