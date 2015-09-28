#include "disktable.h"

#include "diskfile.h"

#include "pairpod.h"

#include <endian.h>	// htobe16
#include <sys/mman.h>	// mmap
#include <fcntl.h>	// open
#include <unistd.h>	// close

//static void null_deleter(const void *ptr){};

DiskTable::DiskTable(DiskTable &&other):
		_mem		(std::move(other._mem		)),
		_size		(std::move(other._size		)),
		_fd		(std::move(other._fd		)),
		_dataCount	(std::move(other._dataCount	)){
	other._mem = nullptr;
	other._size = 0;
}
/*
DiskTable &DiskTable::operator = (DiskTable &&other){
	IArray::operator= (std::move(other));

	close();

	_mem		= std::move(other._mem		);
	_size		= std::move(other._size		);
	_fd		= std::move(other._fd		);
	_datacount	= std::move(other._datacount	);

	other._mem = nullptr;
	other._size = 0;

	return *this;
}
*/
bool DiskTable::open(const StringRef &filename){
	close();

	int fd = ::open(filename.data(), O_RDONLY);

	if (fd < 0)
		return false;

	off_t size2 = lseek(fd, 0, SEEK_END);

	uint64_t size = size2 <= 0 ? 0 : size2;

	if (size == 0 && size < DiskFile::sizeofHeader()){
		::close(fd);
		return false;
	}

	const void *mem = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, /* offset */ 0);

	if (mem == MAP_FAILED){
		::close(fd);
		return false;
	}

	_fd = fd;
	_size = size;
	_mem = mem;

	// memoize count instead to read it from the disk each time
	_dataCount = _getCountFromDisk();

	if (size < DiskFile::sizeofHeader() + _dataCount * sizeof(uint64_t)){
		close();
		return false;
	}

	return true;
}

void DiskTable::close(){
	if (_mem == nullptr || _size == 0)
		return;

	munmap((void *) _mem, _size);
	::close(_fd);

	_mem = nullptr;
	_size = 0;
}

int DiskTable::_cmpAt(uint64_t const index, const StringRef &key) const{
	const PairPOD *p = (const PairPOD *) _getAtFromDisk(index);
	return p->cmp(key.data());
}

size_t DiskTable::_getSize() const{
	return _size - DiskFile::sizeofHeader() - _dataCount * sizeof(uint64_t);
}

uint64_t DiskTable::_getCountFromDisk() const{
	const DiskTableHeader *head = (DiskTableHeader *) _mem;
	return be64toh(head->size);
}

const void *DiskTable::_getAtFromDisk(uint64_t const index) const{
	// index is check in parent

	// TODO: check if we are inside the memory block.

	const char *mem = (char *) _mem;
	const DiskTableHeader *head = (DiskTableHeader *) _mem;

	const uint64_t ptr = be64toh( head->data[index] );
	return & mem[ptr];
}
