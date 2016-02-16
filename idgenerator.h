#ifndef _ID_GENERATOR_H
#define _ID_GENERATOR_H

#include <string>

class IDGeneratorTS{
public:
	std::string next();

};

class IDGeneratorTSHex{
public:
	std::string next();

};

class IDGeneratorDate{
public:
	std::string next();

};

#endif

