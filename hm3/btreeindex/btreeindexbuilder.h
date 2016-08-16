#ifndef _DISK_BTREE_H
#define _DISK_BTREE_H

#include "stringref.h"
#include "btreeindexnode.h"
#include "btreeindexfilenames.h"

#include <fstream>

namespace hm3{
namespace btreeindex{


class BTreeIndexBuilder{
public:
	constexpr static bool MEMSET_UNUSED_NODES	= true;

public:
	BTreeIndexBuilder(const StringRef &filename_indx, const StringRef &filename_data) :
			filename_indx(filename_indx),
			filename_data(filename_data){}

	BTreeIndexBuilder(std::string &&filename_indx, std::string &&filename_data) :
			filename_indx(std::move(filename_indx)),
			filename_data(std::move(filename_data)){}

	BTreeIndexBuilder(const std::string &filename) :
			BTreeIndexBuilder( filenameIndx(filename), filenameData(filename)){}

public:
	template <class LIST>
	bool createFromList(const LIST &list) const;

private:
	class BTreeIndexBuilderHelper_;

private:
	static branch_type calcDepth__(offset_type const count);

	static branch_type calcDepth1__(offset_type const count){
		branch_type result = calcDepth__(count);

		return result > 1 ? result - 1 : 1;
	}

private:
	std::string	filename_indx;
	std::string	filename_data;
};


} // namespace btreeindex
} // namespace


#include "btreeindexbuilder_impl.h"

#endif

