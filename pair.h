#ifndef _PAIR_H
#define _PAIR_H

#include "ipair.h"
#include "ichecksumcalculator.h"

class Pair : public IPair{
public:
	Pair(IChecksumCalculator *checksumCalculator);
	
	virtual const char *getKey() const;
	virtual const char *getValue() const;
	
	virtual int cmp(const char *key) const;
	
	virtual bool valid() const;

private:
	uint8_t _getChecksum() const;

private:
	uint16_t		_keyLen;
	uint32_t		_valLen;
	
	char			_buffer[];
	uint32_t		_bufferLen;

	uint8_t			_checksum;
	
	IChecksumCalculator	*_checksumCalculator = NULL;
};

#endif
