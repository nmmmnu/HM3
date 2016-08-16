#ifndef DISK_FILE_BUILDER_H_
#define DISK_FILE_BUILDER_H_

#include "stringref.h"
#include "diskfilefilenames.h"

//#include <cstdint>

#include <iostream>


namespace hm3{
namespace diskfile{


class DiskFileBuilder{
public:
	DiskFileBuilder(const StringRef &filename_meta, const StringRef &filename_indx, const StringRef &filename_data) :
			filename_meta(filename_meta),
			filename_indx(filename_indx),
			filename_data(filename_data){}

	DiskFileBuilder(std::string &&filename_meta, std::string &&filename_indx, std::string &&filename_data) :
			filename_meta(std::move(filename_meta)),
			filename_indx(std::move(filename_indx)),
			filename_data(std::move(filename_data)){}

	DiskFileBuilder(const std::string &filename) :
			DiskFileBuilder( filenameMeta(filename), filenameIndx(filename), filenameData(filename)){}


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
				bool keepTombstones) const;

private:
	std::string filename_meta;
	std::string filename_indx;
	std::string filename_data;
};


} // namespace diskfile
} // namespace


#include "diskfilebuilder_impl.h"

#endif

