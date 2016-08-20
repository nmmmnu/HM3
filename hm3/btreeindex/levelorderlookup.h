#ifndef LEVEL_ORDER_LOOKUP_H_
#define LEVEL_ORDER_LOOKUP_H_

#include "btreeindexnode.h"

namespace hm3{
namespace btreeindex{


template<level_type LEVELS>
class LevelOrderLookup{
public:
	constexpr static branch_type SIZE   = (1 << LEVELS) - 1;

public:
	LevelOrderLookup(){
		for(level_type level = 0; level < LEVELS; ++level)
			reorder_(0, SIZE, level);
	}

	branch_type fw(branch_type const index) const{
		return fw_[index];
	}

	branch_type bw(branch_type const index) const{
		return bw_[index];
	}

	branch_type size() const{
		return SIZE;
	}

private:
	void reorder_(branch_type begin, branch_type end, level_type level, level_type this_level = 0);

	void push_(branch_type const value){
		fw_[pos_] = value;
		bw_[pos_] = 0;
		//bw_[value] = pos_;
		++pos_;
	}

private:
	branch_type fw_[SIZE];
	branch_type bw_[SIZE];

	branch_type pos_ = 0;
};




template<level_type LEVELS>
void LevelOrderLookup<LEVELS>::reorder_(branch_type const begin, branch_type const end, level_type const level, level_type const this_level){
	if (begin >= end){
		// size is guaranteed to be "aligned",
		// no push_(NIL) needed...
		return;
	}

	branch_type const mid = branch_type( begin + ((end - begin) >> 1) );

	if (this_level == level){
		push_(mid);
		return;
	}

	reorder_(begin,                mid, level, level_type(this_level + 1));
	reorder_(branch_type(mid + 1), end, level, level_type(this_level + 1));
}



} // namespace btreeindex
} // namespace

#endif

