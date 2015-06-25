#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "ilist.h"
#include "iarray.h"

class VectorList : virtual public IList, virtual public IArray{
public:
	static const size_t REALLOC_SIZE = 1 * sizeof(void *);

public:
	VectorList(size_t reallocChunkSize = 0);
	virtual ~VectorList() override;

	virtual void removeAll() override;

	virtual bool put(Pair *pair) override;
	virtual bool remove(const char *key) override;

	virtual OPair getAt(uint64_t index) const override;

	virtual uint64_t getCount() const override;
	virtual size_t getSize() const override;

private:
	size_t		_reallocSize;

	Pair		**_buffer;
	size_t		_bufferSize;

	uint64_t	_dataCount;
	size_t		_dataSize;

	uint64_t	_itPos;

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

#endif
