#ifndef LEVEL_ORDER_LOOKUP_H_
#define LEVEL_ORDER_LOOKUP_H_

#include "btreeindexnode.h"

#include <array>

namespace hm3{
namespace btreeindex{


template<level_type LEVELS>
class LevelOrderLookup{
public:
	constexpr static branch_type SIZE   = (1 << LEVELS) - 1;

	using LookupTable = std::array<branch_type, SIZE>;

public:
	LevelOrderLookup(){
		branch_type push_pos = 0;

		for(level_type level = 0; level < LEVELS; ++level)
			reorderLevel_(push_pos, level);
	}

	const LookupTable &get() const{
		return fwTable_;
	}

private:
	void reorderLevel_(branch_type &push_pos, level_type const level){
		reorder_(push_pos, 0, SIZE, level, 0);
	}

	void push_back(branch_type &push_pos, branch_type const data){
		fwTable_[push_pos++] = data;
	}

	void reorder_(branch_type &push_pos, branch_type begin, branch_type end, level_type level, level_type this_level);

private:
	LookupTable fwTable_;

};



template<level_type LEVELS>
void LevelOrderLookup<LEVELS>::reorder_(branch_type &push_pos, branch_type const begin, branch_type const end, level_type const level, level_type const this_level){
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



} // namespace btreeindex
} // namespace

#endif

