#include "disktable.h"

#include "diskfile.h"

#include <endian.h>	// htobe16
#include <sys/mman.h>	// mmap
#include <fcntl.h>	// open
#include <unistd.h>	// close

//static void null_deleter(const void *ptr){};

DiskTable::DiskTable(DiskTable &&other):
		_mem		(other._mem		),
		_size		(other._size		),
		_fd		(other._fd		),
		_datacount	(other._datacount	){
	other._mem = nullptr;
	other._size = 0;
}

bool DiskTable::open(const char *filename){
	close();

	int fd = ::open(filename, O_RDONLY);

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
	_datacount = _getCountFromDisk();

	if (size < DiskFile::sizeofHeader() + _datacount * sizeof(uint64_t)){
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

Pair DiskTable::_getAt(uint64_t const index) const{
/*
	auto p =  _getAtFromDisk(index);

	std::shared_ptr<const PairPOD> pod{ p, null_deleter };

	return Pair{ std::move(pod) };
*/
	return _getAtFromDisk(index);
}


int DiskTable::_cmpAt(uint64_t const index, const char *key) const{
	const PairPOD *p = _getAtFromDisk(index);
	return p->cmp(key);
}

uint64_t DiskTable::_getCount() const{
	return _datacount;
};

size_t DiskTable::_getSize() const{
	return _size - DiskFile::sizeofHeader() - _datacount * sizeof(uint64_t);
}

uint64_t DiskTable::_getCountFromDisk() const{
	const DiskTableHeader *head = (DiskTableHeader *) _mem;
	return be64toh(head->size);
}

const PairPOD *DiskTable::_getAtFromDisk(uint64_t const index) const{
	// index is check in parent

	// TODO: check if we are inside the memory block.

	const char *mem = (char *) _mem;
	const DiskTableHeader *head = (DiskTableHeader *) _mem;

	const uint64_t ptr = be64toh( head->data[index] );
	return (PairPOD *) & mem[ptr];
}
