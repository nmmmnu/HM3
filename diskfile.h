#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstdint>

#include <iostream>

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

private:
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

