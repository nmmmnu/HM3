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
	const char	logo[8] = DISK_TABLE_LOGO;		// 8
	const uint8_t	version = DISK_TABLE_VERSION_INT;	// 1
	uint64_t	size;					// 8
} __attribute__((__packed__));

// ==============================

namespace DiskFile{
	constexpr static const char *DOT_META = ".meta";
	constexpr static const char *DOT_INDX = ".indx";
	constexpr static const char *DOT_DATA = ".data";

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

	inline std::string filenameMeta(const std::string &filename){
		return filename + DOT_META;
	}

	inline std::string filenameIndx(const std::string &filename){
		return filename + DOT_INDX;
	}

	inline std::string filenameData(const std::string &filename){
		return filename + DOT_DATA;
	}

	template <class LIST>
	bool create(const LIST &list, const std::string &filename){
		return create(list,
				filenameMeta(filename),
				filenameIndx(filename),
				filenameData(filename));
	}

}; // namespace DiskFile

// ==============================

#include "diskfile_implementation.h"

#endif

