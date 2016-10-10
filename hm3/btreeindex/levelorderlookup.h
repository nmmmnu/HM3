#ifndef LEVEL_ORDER_LOOKUP_H_
#define LEVEL_ORDER_LOOKUP_H_

#include "btreeindextypes.h"

namespace hm3{
namespace btreeindex{



template<level_type LEVELS>
class LevelOrderLookup{
public:
	constexpr static branch_type size   = (1 << LEVELS) - 1;

	branch_type value[size];

public:
	constexpr LevelOrderLookup() : value(){
		branch_type push_pos = 0;

		for(level_type level = 0; level < LEVELS; ++level)
			reorderLevel_(push_pos, level);
	}

private:
	constexpr void reorderLevel_(branch_type &push_pos, level_type const level){
		reorder_(push_pos, 0, size, level, 0);
	}

	constexpr void push_back(branch_type &push_pos, branch_type const data){
		value[push_pos++] = data;
	}

	constexpr void reorder_(branch_type &push_pos, branch_type begin, branch_type end, level_type level, level_type this_level){
		if (begin >= end){
			// size is guaranteed to be "aligned",
			// no push_(NIL) needed...
			return;
		}

		branch_type const mid = branch_type( begin + ((end - begin) >> 1) );

		if (this_level == level){
			return push_back(push_pos, mid);
		}

		reorder_(push_pos, begin,                mid, level, level_type(this_level + 1));
		reorder_(push_pos, branch_type(mid + 1), end, level, level_type(this_level + 1));
	}
};



} // namespace btreeindex
} // namespace

#endif

