#include "iiterator.h"

uint64_t IIterator::iteratorCount(){
	uint64_t count = 0;

	for(auto p = first(); p; p = next())
		++count;

	return count;
}

void IIterator::print(uint64_t limit){
	uint64_t count = 0;

	for(auto p = first(); p; p = next()){
		p->print();

		if (++count >= limit)
			break;
	}
};

