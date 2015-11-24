#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstdint>

#include <iostream>
#include <fstream>

#define DISK_TABLE_TITLE	"ZUSE"
#define DISK_TABLE_VERSION	"002"
#define DISK_TABLE_VERSION_INT	2

#define DISK_TABLE_LOGO		DISK_TABLE_TITLE DISK_TABLE_VERSION

struct DiskTableHeader{
	const char	logo[8]	= DISK_TABLE_LOGO;		// 8
	const uint8_t	version	= DISK_TABLE_VERSION_INT;	// 1
	uint64_t	size;					// 8
	uint8_t		sorted;					// 1
} __attribute__((__packed__));

// ==============================

class DiskFile{
public:
	constexpr static uint8_t NOT_SORTED	= 0;
	constexpr static uint8_t SORTED		= 1;

	constexpr static const char *DOT_INDX = ".indx";
	constexpr static const char *DOT_DATA = ".data";

public:
	template <class LIST>
	static bool create(const LIST &list,
				const StringRef &filename_meta, const StringRef &filename_index, const StringRef &filename_data,
				bool keepInvalid, bool keepTombstones);

	template <class LIST>
	static  bool create(const LIST &list, const std::string &filename, bool keepInvalid = true, bool keepTombstones = true){
		return create(list,
				filename,
				filenameIndx(filename),
				filenameData(filename),
				keepInvalid, keepTombstones);
	}

	template <class LIST>
	static bool writeListToFile(const LIST &list,
				std::ofstream &file_meta, std::ofstream &file_index, std::ofstream &file_data,
				bool keepInvalid, bool keepTombstones);

public:
	static std::string filenameIndx(const std::string &filename){
		return filename + DOT_INDX;
	}

	static std::string filenameData(const std::string &filename){
		return filename + DOT_DATA;
	}
};

// ==============================

#include "diskfile_implementation.h"

#endif

