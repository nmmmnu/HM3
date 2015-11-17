#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstddef>	// ofsetof
#include <cstdint>

#include <iostream>
#include <fstream>

#define DISK_TABLE_TITLE	"ZUSE"
#define DISK_TABLE_VERSION	"002"
#define DISK_TABLE_VERSION_INT	2

#define DISK_TABLE_LOGO		DISK_TABLE_TITLE DISK_TABLE_VERSION

struct DiskTableHeader{
	const char	logo[8] = DISK_TABLE_LOGO;		// 8
	const uint8_t	version = DISK_TABLE_VERSION_INT;	// 1
	uint64_t	size;					// 8
} __attribute__((__packed__));

// ==============================

namespace DiskFile{
	template <class LIST>
	bool create(const LIST &list,
		const StringRef &filename_head,
		const StringRef &filename_index,
		const StringRef &filename_data);

	template <class LIST>
	static bool writeListToFile(const LIST &list,
		std::ofstream &file_head,
		std::ofstream &file_index,
		std::ofstream &file_data);
}; // namespace DiskFile

// ==============================

#include "diskfile_implementation.h"

#endif

