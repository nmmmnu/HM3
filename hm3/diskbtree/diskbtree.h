#ifndef _DISK_BTREE_H
#define _DISK_BTREE_H

#include "stringref.h"
#include "diskbtreenode.h"

#include <fstream>

namespace hm3{
namespace diskbtree{


class DiskBTree{
public:
	constexpr static const char *DOT_INDX	= ".bindx";
	constexpr static const char *DOT_DATA	= ".btree";

	constexpr static auto BRANCHES		= DiskBTreeNode::BRANCHES;
	constexpr static auto VALUES		= DiskBTreeNode::VALUES;

	using branch_type			= DiskBTreeNode::branch_type;
	using offset_type			= DiskBTreeNode::offset_type;

	using Node				= DiskBTreeNode::Node;
	using NodeValue				= DiskBTreeNode::NodeValue;

public:
	DiskBTree(const StringRef &filename_indx, const StringRef &filename_data) :
			filename_indx(filename_indx),
			filename_data(filename_data){}

	DiskBTree(std::string &&filename_indx, std::string &&filename_data) :
			filename_indx(std::move(filename_indx)),
			filename_data(std::move(filename_data)){}

	DiskBTree(const std::string &filename) :
			DiskBTree( filenameIndx(filename), filenameData(filename)){}

public:
	static std::string filenameIndx(const std::string &filename){
		return filename + DOT_INDX;
	}

	static std::string filenameData(const std::string &filename){
		return filename + DOT_DATA;
	}

public:
	template <class LIST>
	bool createFromList(const LIST &list) const;

private:
	template <class LIST>
	void reorder_(const LIST &list,
				typename LIST::size_type begin, typename LIST::size_type end,
				branch_type level, branch_type this_level,
					size_t &current,
					std::ofstream &file_indx,
					std::ofstream &file_data) const;

	void injectEmptyNode_(branch_type level, branch_type this_level,
					std::ofstream &file_indx) const;

private:
	constexpr
	static branch_type ln__(offset_type const count, branch_type const branches);

private:
	std::string	filename_indx;
	std::string	filename_data;
};


} // namespace diskfile
} // namespace


#include "diskbtree_impl.h"

#endif

