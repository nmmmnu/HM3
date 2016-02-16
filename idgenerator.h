#ifndef _ID_GENERATOR_H
#define _ID_GENERATOR_H

#include <string>
#include <ostream>
#include <cstdint>

class IDGeneratorTS{
public:
	IDGeneratorTS(bool hex = true) : _hex(hex){};

	std::string operator()() const;

private:
	void _format(std::ostream &buff, uint32_t value) const;

private:
	bool _hex;
};



class IDGeneratorDate{
public:
	std::string operator()() const;
};

#endif

