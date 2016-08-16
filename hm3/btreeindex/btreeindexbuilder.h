#ifndef _DISK_BTREE_H
#define _DISK_BTREE_H

#include "stringref.h"
#include "btreeindexnode.h"
#include "btreeindexfilenames.h"

#include <fstream>

namespace hm3{
namespace btreeindex{


template <class LIST>
class BTreeIndexBuilder{
public:
	constexpr static bool MEMSET_UNUSED_VALUES	= true;

public:
	using size_type = typename LIST::size_type;

public:
	bool createFromList(const StringRef &filename, const LIST &list);

private:
	static branch_type calcDepth__(size_type count);

	static branch_type calcDepth1__(size_type const count){
		branch_type result = calcDepth__(count);

		return result > 1 ? result - 1 : 1;
	}

private:
	void injectEmptyNode_(branch_type level, branch_type this_level);

	void injectValue_(const LIST &list, size_type index);

	void reorder(const LIST &list,
				size_type begin, size_type end,
				branch_type level, branch_type this_level = 0);

private:
	std::ofstream	file_indx_;
	std::ofstream	file_data_;
	size_t		current_		= 0;
};


} // namespace btreeindex
} // namespace


#include "btreeindexbuilder_impl.h"

#endif

