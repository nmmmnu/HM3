#ifndef LEVEL_ORDER_LOOKUP_H_
#define LEVEL_ORDER_LOOKUP_H_

template<typename size_type, unsigned char LEVELS>
class LevelOrderLookup{
public:
	using level_type = unsigned char;
	
	constexpr static size_type size = (1 << LEVELS) - 1;

	size_type value[size];

public:
	constexpr LevelOrderLookup() : value(){
		size_type push_pos = 0;

		for(level_type level = 0; level < LEVELS; ++level)
			reorderLevel_(push_pos, level);
	}

private:
	constexpr void reorderLevel_(size_type &push_pos, level_type const level){
		reorder_(push_pos, 0, size, level, 0);
	}

	constexpr void push_back(size_type &push_pos, size_type const data){
		value[push_pos++] = data;
	}

	constexpr void reorder_(size_type &push_pos, size_type const begin, size_type const end, level_type const level, level_type const this_level){
		if (begin >= end){
			// size is guaranteed to be "aligned",
			// no push_(NIL) needed...
			return;
		}

		size_type const mid = size_type( begin + ((end - begin) >> 1) );

		if (this_level == level){
			return push_back(push_pos, mid);
		}

		reorder_(push_pos, begin,              mid, level, level_type(this_level + 1));
		reorder_(push_pos, size_type(mid + 1), end, level, level_type(this_level + 1));
	}
};


#endif

