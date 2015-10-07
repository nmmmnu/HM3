#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "iversion.h"
#include "pair.h"

class IList : public IVersion{
public:
	typedef uint32_t count_type;

public:
	void print(){}
};

#endif

