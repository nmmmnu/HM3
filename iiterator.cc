#include "iiterator.h"

void IIterator::print(uint64_t limit){
	uint64_t count = 0;
	const Pair *p;
	for(p = first(); p; p = next()){
		p->print();

		if (++count >= limit)
			break;
	}
};

