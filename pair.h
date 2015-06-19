#ifndef _PAIR_H
#define _PAIR_H

#include "ipair.h"
#include "ichecksumcalculator.h"

typedef struct _PairBuffer PairBuffer;

class Pair : public IPair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:
	Pair(IChecksumCalculator *checksumCalculator = NULL);

	virtual ~Pair();

	bool create(const char *key, const void *value, size_t valLen, uint32_t expires = 0);
	bool create(const char *key, const char *value, uint32_t expires = 0);
	bool create(const char *key, uint32_t expires = 0);
	void destroy();

	void load(PairBuffer *pb);

	virtual const char *getKey() const;
	virtual const char *getVal() const;

	virtual bool valid() const;

	virtual size_t getSize() const;

	virtual void print() const;

	virtual bool saveToFile(FILE *F) const;

private:
	size_t _sizeofBuffer() const;

	void _clear();

private:
	uint8_t _getChecksum() const;

private:
	PairBuffer		*_pb = NULL;
	bool			_pbDestroy;
	IChecksumCalculator	*_checksumCalculator = NULL;
};

#endif
