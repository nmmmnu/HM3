#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "irolist.h"

#include <stddef.h>	// ofsetof
#include <stdint.h>

#include <iostream>
#include <fstream>

struct DiskTableHeader{
	uint64_t	size;		// 8
	uint64_t	data[1];	// 8, dynamic
} __attribute__((__packed__));

// ==============================

class DiskFile{
public:
	static bool create(const char *filename, IIterator &it, uint64_t datacount);
	static bool create(const char *filename, IROList &list);

	constexpr 
	static size_t sizeofHeader();

private:
	static bool _writeIteratorToFile(IIterator &it, uint64_t datacount, std::ofstream &file);

};

// ==============================

inline bool DiskFile::create(const char *filename, IROList &list){
	return create(filename, list.getIterator(), list.getCount());
}

constexpr
inline size_t DiskFile::sizeofHeader(){
	return offsetof(DiskTableHeader, data);
}

#endif

