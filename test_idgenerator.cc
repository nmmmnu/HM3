#include "idgenerator.h"

#include <iostream>
#include <iomanip>

template <class T>
void test(const char *title, unsigned count = 10){
	T id;
	std::cout	<< title
			<< std::endl;
	for(unsigned i = 0; i < count; ++i)
		std::cout	<< std::setw (3) << i
				<< " => "
				<< id.next() 
				<< std::endl;
}

int main(){
	test<IDGeneratorTS>("IDGeneratorTS");
	test<IDGeneratorTSHex>("IDGeneratorTSHex");
	test<IDGeneratorDate>("IDGeneratorDate");
}

