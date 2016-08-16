#ifndef DISK_FILE_BUILDER_H_
#define DISK_FILE_BUILDER_H_

#include "stringref.h"
#include "diskfilefilenames.h"

#include "stringref.h"

#include <iostream>


namespace hm3{
namespace diskfile{


class DiskFileBuilder{
public:
	template <class LIST>
	bool createFromList(const StringRef &filename, const LIST &list,
				bool keepTombstones) const{
		return createFromIterator(filename, list.begin(), list.end(), keepTombstones);
	}

	template <class ITERATOR>
	bool createFromIterator(const StringRef &filename, const ITERATOR &begin, const ITERATOR &end,
				bool keepTombstones) const;

private:
	template <class ITERATOR>
	bool _writeIteratorToFile(const ITERATOR &begin, const ITERATOR &end,
				std::ofstream &file_meta, std::ofstream &file_index, std::ofstream &file_data,
				bool keepTombstones) const;
};


} // namespace diskfile
} // namespace


#include "diskfilebuilder_impl.h"

#endif

