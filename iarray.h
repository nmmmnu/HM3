#ifndef _IARRAY_H
#define _IARRAY_H

#include "pair.h"
#include "iiterator.h"
#include "icountable.h"

class IArray : virtual public IIterator, virtual public ICountable{
public:
	virtual ~IArray(){};

	virtual const Pair *getAt(uint64_t index) const = 0;

public:
	virtual bool rewind(const char *key = NULL) override;
	virtual const Pair *next() override;

protected:
	virtual int locatePosition(const char *key, uint64_t *index) const = 0;

private:
	uint64_t	_itPos = 0;

};

#endif

