#ifndef _BTREE_INDEX_BUILDER_H
#define _BTREE_INDEX_BUILDER_H

#include "stringref.h"
#include "btreeindexnode.h"
#include "btreeindexfilenames.h"
#include "levelorderlookup.h"

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
	static level_type calcDepth__(size_type count);

	static level_type calcDepth1__(size_type const count){
		level_type const result = calcDepth__(count);

		return level_type( result > 1 ? result - 1 : 1 );
	}

private:
	void injectEmptyNode_(level_type level, level_type this_level);

	void injectValue_(const LIST &list, size_type index);

	void reorder(const LIST &list,
				size_type begin, size_type end,
				level_type level, level_type this_level = 0);

private:
	static constexpr LevelOrderLookup<branch_type, NODE_LEVELS> llHolder_{};

private:
	std::ofstream	file_indx_;
	std::ofstream	file_data_;
	size_t		current_	= 0;
	branch_type	levels_;
};


} // namespace btreeindex
} // namespace


#include "btreeindexbuilder_impl.h"

#endif

