#ifndef _DISK_FILE_H
#define _DISK_FILE_H

#include "stringref.h"

#include <cstdint>

#include <iostream>


namespace hm3{
namespace diskfile{


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
			DiskFile( filenameMeta(filename), filenameIndx(filename), filenameData(filename)){}

public:
	static std::string filenameMeta(const std::string &filename){
		return filename;
	}

	static std::string filenameIndx(const std::string &filename){
		return filename + DOT_INDX;
	}

	static std::string filenameData(const std::string &filename){
		return filename + DOT_DATA;
	}

public:
	template <class LIST>
	bool createFromList(const LIST &list,
				bool keepTombstones) const{
		return createFromIterator(list.begin(), list.end(), keepTombstones);
	}

	template <class ITERATOR>
	bool createFromIterator(const ITERATOR &begin, const ITERATOR &end,
				bool keepTombstones) const;

private:
	template <class ITERATOR>
	bool _writeIteratorToFile(const ITERATOR &begin, const ITERATOR &end,
				std::ofstream &file_meta, std::ofstream &file_index, std::ofstream &file_data,
				std::ofstream &file_keyindex, std::ofstream &file_keydata,
				bool keepTombstones) const;

private:
	std::string filename_meta;
	std::string filename_indx;
	std::string filename_data;
};


} // namespace diskfile
} // namespace


#include "diskfile_impl.h"

#endif

