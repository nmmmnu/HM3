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
		return mem_ == nullptr;
	}

	const void *mem() const{
		return mem_;
	}

	const void *safeAccess(size_t const offset) const{
		if (mem_ == nullptr)
			return nullptr;

		if (size_ < offset)
			return nullptr;

		const char *memc = (const char *) mem_;

		return & memc[offset];
	}

	const void *safeAccess(const void *ptr) const{
		const char *ptrc = (const char *)  ptr;
		const char *memc = (const char *) mem_;

		if (ptrc < memc)
			return nullptr;

		/*
		// old version, this is wrong:
		return safeAccess(ptrc - (size_t) memc);
		*/

		/* long int */ auto const offset = ptrc - memc;

		return safeAccess( (size_t) offset);
	}

	size_t size() const{
		return size_;
	}

private:
	const void	*mem_		= nullptr;
	size_t		size_		= 0;

	int		fd_;
};

#endif

