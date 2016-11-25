#ifndef _MMAP_FILE_H
#define _MMAP_FILE_H

#include "stringref.h"

#include <sys/mman.h>	// mmap

class MMAPFile{
public:
	MMAPFile(int const madvise = MADV_NORMAL) :
				madvise_(madvise){}

	MMAPFile(MMAPFile &&other);

	~MMAPFile(){
		close();
	}

	bool open(const StringRef &filename){
		return openRO(filename);
	}

	bool openRO(const StringRef &filename);
	bool openRW(const StringRef &filename);

	void close();

	operator bool() const{
		return mem_ != nullptr;
	}

	const void *mem() const{
		return mem_;
	}

	void *mem() {
		return mem_;
	}

	size_t size() const{
		return size_;
	}

public:
	static bool createFile(const StringRef &filename, size_t size = 0);

private:
	bool open_(const StringRef &filename, int mode, int prot);

private:
	void	*mem_		= nullptr;
	size_t	size_		= 0;

	int	fd_;

	int	madvise_;

};

#endif


