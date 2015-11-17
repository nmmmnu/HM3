#ifndef _MMAP_FILE_H
#define _MMAP_FILE_H

#include "stringref.h"

class MMAPFile{
public:
	MMAPFile() = default;

	~MMAPFile(){
		close();
	}

	bool open(const StringRef &filename);

	void close();

	const void *mem() const{
		return _mem;
	}

	size_t size() const{
		return _size;
	}

private:
	const void	*_mem		= nullptr;
	size_t		_size		= 0;

	int		_fd;
};

#endif

