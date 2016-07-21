#include "mmapfile.h"

#include <sys/mman.h>	// mmap
#include <fcntl.h>	// open
#include <unistd.h>	// close

MMAPFile::MMAPFile(MMAPFile &&other) :
		mem_(	std::move(other.mem_)),
		size_(	std::move(other.size_)),
		fd_(	std::move(other.fd_)){
	other.mem_ = nullptr;
	other.size_ = 0;
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

	/* const */ void *mem = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, /* offset */ 0);

	if (random_madvise_)
		madvise(mem, size, MADV_RANDOM);

	if (mem == MAP_FAILED){
		::close(fd);
		return false;
	}

	fd_ = fd;
	size_ = size;
	mem_ = mem;

	return true;
}

void MMAPFile::close(){
	if (mem_ == nullptr)
		return;

	munmap((void *) mem_, size_);
	::close(fd_);

	mem_ = nullptr;
	size_ = 0;
}

