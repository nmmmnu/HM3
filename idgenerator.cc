#include "idgenerator.h"

#include "mytime.h"

#include <iomanip>
#include <sstream>

void IDGeneratorTS::_format(std::ostream &buff, uint32_t value) const{
	if (_hex){
		buff	<< std::setfill ('0') << std::setw (8) << std::hex;
	}else{
		buff	<< std::setfill ('0') << std::setw (10);
	}

	buff << value;
}

std::string IDGeneratorTS::operator()() const{
	std::ostringstream buff;

	auto now = MyTime::now();

	_format(buff, MyTime::uncombine(now));

	buff << ".";

	_format(buff, MyTime::uncombine2(now));

	return buff.str();
}

std::string IDGeneratorDate::operator()() const{
	std::ostringstream buff;

	static const char *FORMAT = MyTime::DATA_FORMAT_NUMBER;

	buff	<< MyTime::toString(MyTime::now(), FORMAT)
		<< "."
		<< std::setfill ('0') << std::setw (10)
		<< MyTime::uncombine2(MyTime::now());


	return buff.str();
}

