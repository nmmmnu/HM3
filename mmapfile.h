#ifndef _MMAP_FILE_H
#define _MMAP_FILE_H

#include "stringref.h"

class MMAPFile{
public:
	MMAPFile() = default;
	MMAPFile(MMAPFile &&other);

	~MMAPFile(){
		close();
	}

	bool open(const StringRef &filename);

	void close();

	operator bool(){
		return _mem == nullptr;
	}

	const void *mem() const{
		return _mem;
	}

	const void *safeAccess(size_t const offset) const{
		if (_mem == nullptr)
			return nullptr;

		const char *memc = (const char *) _mem;

		if (_size < offset)
			return nullptr;

		return & memc[offset];
	}

	const void *safeAccess(const void *ptr) const{
		const char *ptrc = (const char *)  ptr;
		const char *memc = (const char *) _mem;

		if (ptrc < memc)
			return nullptr;

		return safeAccess(ptrc - memc);
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

