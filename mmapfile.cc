#include "mmapfile.h"

#include <sys/mman.h>	// mmap
#include <fcntl.h>	// open
#include <unistd.h>	// close

MMAPFile::MMAPFile(MMAPFile &&other) :
		_mem(	std::move(other._mem)),
		_size(	std::move(other._size)),
		_fd(	std::move(other._fd)){
	other._mem = nullptr;
	other._size = 0;
}

bool MMAPFile::open(const StringRef &filename){
	close();

	int fd = ::open(filename.data(), O_RDONLY);

	if (fd < 0)
		return false;

	off_t size2 = lseek(fd, 0, SEEK_END);

	size_t size = size2 <= 0 ? 0 : (size_t) size2;

	if (size == 0){
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

	return true;
}

void MMAPFile::close(){
	if (_mem == nullptr)
		return;

	munmap((void *) _mem, _size);
	::close(_fd);

	_mem = nullptr;
	_size = 0;
}

