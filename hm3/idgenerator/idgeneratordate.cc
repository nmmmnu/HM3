#include "idgeneratordate.h"

#include "mytime.h"

#include <iomanip>
#include <sstream>

namespace hm3{
namespace idgenerator{


std::string IDGeneratorDate::operator()() const{
	std::ostringstream buff;

	static const char *FORMAT = MyTime::DATA_FORMAT_NUMBER;

	buff	<< MyTime::toString(MyTime::now(), FORMAT)
		<< "."
		<< std::setfill('0') << std::setw(10)
		<< MyTime::uncombine2(MyTime::now());


	return buff.str();
}


} // namespace idgenerator
} // namespace
