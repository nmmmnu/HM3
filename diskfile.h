#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "itable.h"

#include "stringref.h"

#include <cstddef>	// ofsetof
#include <cstdint>

#include <iostream>
#include <fstream>

#define DISK_TABLE_TITLE	"ZUSE"
#define DISK_TABLE_VERSION	"001"

#define DISK_TABLE_LOGO		DISK_TABLE_TITLE DISK_TABLE_VERSION
#define DISK_TABLE_SIZE		256

#define DISK_TABLE_PADDING	DISK_TABLE_SIZE - 8 - 2 * sizeof(uint64_t)

struct DiskTableHeader{
	const char	logo[8] =  DISK_TABLE_LOGO;	// 8
	uint64_t	size;				// 8
	char		padding[DISK_TABLE_PADDING];	// calculated
	uint64_t	data[1];			// 8, dynamic
} __attribute__((__packed__));

// ==============================

class DiskFile{
public:
	static bool create(const StringRef &filename, IIterator &it, uint64_t datacount);
	static bool create(const StringRef &filename, ITable &table);

	constexpr
	static size_t sizeofHeader();

private:
	static bool _writeIteratorToFile(IIterator &it, uint64_t datacount, std::ofstream &file);

};

// ==============================

inline bool DiskFile::create(const StringRef &filename, ITable &table){
	auto iterator = table.getIterator();
	return create(filename, *iterator, table.getCount());
}

constexpr
inline size_t DiskFile::sizeofHeader(){
	return offsetof(DiskTableHeader, data);
}

#endif

