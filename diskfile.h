#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstddef>	// ofsetof
#include <cstdint>

#include <iostream>
#include <fstream>

#define DISK_TABLE_TITLE	"ZUSE"
#define DISK_TABLE_VERSION	"001"

#define DISK_TABLE_LOGO		DISK_TABLE_TITLE DISK_TABLE_VERSION
#define DISK_TABLE_SIZE		256

#define DISK_TABLE_PADDING	DISK_TABLE_SIZE			\
				- sizeof(char) * 8 /* logo */	\
				- sizeof(uint64_t) /* size */	\
				- sizeof(uint64_t) /* data */

struct DiskTableHeader{
	const char	logo[8] =  DISK_TABLE_LOGO;	// 8
	uint64_t	size;				// 8
	char		padding[DISK_TABLE_PADDING];	// calculated
	uint64_t	data[1];			// 8, dynamic
} __attribute__((__packed__));

// ==============================

class DiskFile{
public:
	template <class LIST>
	static bool create(const LIST &list, const StringRef &filename);

	constexpr
	static size_t sizeofHeader(){
		return offsetof(DiskTableHeader, data);
	}

private:
	template <class LIST>
	static bool _writeListToFile(const LIST &list, std::ofstream &file);

};

// ==============================

#include "diskfile_implementation.h"

#endif

