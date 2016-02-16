#include "idgenerator.h"

#include "mytime.h"

#include <sstream>
#include <iomanip>

std::string IDGeneratorTS::next(){
	std::ostringstream buff;

	buff	<< std::setfill ('0') << std::setw (10)
		<< MyTime::uncombine(MyTime::now())
		<< "."
		<< std::setfill ('0') << std::setw (10)
		<< MyTime::uncombine2(MyTime::now());
	
	return buff.str();
}

std::string IDGeneratorTSHex::next(){
	std::ostringstream buff;

	buff	<< std::setfill ('0') << std::setw (8) << std::hex
		<< MyTime::uncombine(MyTime::now())
		<< "."
		<< std::setfill ('0') << std::setw (8) << std::hex
		<< MyTime::uncombine2(MyTime::now());
	
	return buff.str();
}

std::string IDGeneratorDate::next(){	
	std::ostringstream buff;
	
	const char *FORMAT = MyTime::DATA_FORMAT_NUMBER;

	buff	<< MyTime::toString(MyTime::now(), FORMAT)
		<< "."
		<< std::setfill ('0') << std::setw (10)	// << std::hex
		<< MyTime::uncombine2(MyTime::now());
	

	return buff.str();
}

