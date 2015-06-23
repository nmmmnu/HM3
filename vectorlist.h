#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "ilist.h"
#include "iarray.h"

class VectorList : virtual public IList, virtual public IArray{
public:
	static const size_t REALLOC_SIZE = 1 * sizeof(void *);

public:
	VectorList(size_t reallocChunkSize = 0);
	virtual ~VectorList();

	virtual void removeAll() override;

	virtual bool put(Pair *pair) override;
	virtual const Pair *get(const char *key) const override;
	virtual bool remove(const char *key) override;

	virtual const Pair *getAt(uint64_t index) const override;

	virtual uint64_t getCount() const override;
	virtual size_t getSize() const override;

private:
	size_t		_reallocSize;

	Pair		**_buffer;
	size_t		_bufferSize;

	uint64_t	_dataCount;
	size_t		_dataSize;

	uint64_t	_itPos;

protected:
	virtual int locatePosition(const char *key, uint64_t *index) const override final;

private:
	void _clear(bool alsoFree = false);

	int _locatePositionBSearch(const char *key, uint64_t *index) const;

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

#endif
