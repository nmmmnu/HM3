#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstdint>

#include <iostream>

// ==============================

class DiskFileHeader{
public:
	constexpr static const size_t	TITLE_SIZE	= 8;
	constexpr static const char	*TITLE		= "ZUSE" "003"; // \0 at the end
	constexpr static const uint8_t	VERSION		= 3;

	constexpr static const uint8_t	HEADER_NOT_SORTED	= 0;
	constexpr static const uint8_t	HEADER_SORTED		= 1;

public:
	struct PODBase{
		char		logo[TITLE_SIZE];	// 8
		uint8_t		version;	// 1
		uint64_t	size;		// 8
	} __attribute__((__packed__));

	struct POD{
		char		logo[8];	// 8
		uint8_t		version;	// 1
		uint64_t	size;		// 8
		// base ends here
		uint64_t	created;	// 8
		uint8_t		sorted;		// 1
		uint64_t	tombstones;	// 8
		uint64_t	createdMin;	// 8
		uint64_t	createdMax;	// 8
	} __attribute__((__packed__));

public:
	static POD create(size_t const datacount, size_t const tombstones,
				uint64_t const createdMin, uint64_t const createdMax );

};

// ==============================

class DiskFile{
public:
	constexpr static const char *DOT_INDX = ".indx";
	constexpr static const char *DOT_DATA = ".data";

public:
	DiskFile(const StringRef &filename_meta, const StringRef &filename_indx, const StringRef &filename_data) :
			filename_meta(filename_meta),
			filename_indx(filename_indx),
			filename_data(filename_data){}

	DiskFile(const std::string &filename) :
			DiskFile(filename, filenameIndx(filename), filenameData(filename)){}

public:
	static std::string filenameIndx(const std::string &filename){
		return filename + DOT_INDX;
	}

	static std::string filenameData(const std::string &filename){
		return filename + DOT_DATA;
	}

public:
	template <class LIST>
	bool createFromList(const LIST &list,
				bool keepInvalid, bool keepTombstones) const;

	template <class LIST>
	bool writeListToFile(const LIST &list,
				std::ofstream &file_meta, std::ofstream &file_index, std::ofstream &file_data,
				bool keepInvalid, bool keepTombstones) const;

private:
	std::string filename_meta;
	std::string filename_indx;
	std::string filename_data;
};

// ==============================

#include "diskfile_implementation.h"

#endif

