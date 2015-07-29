#ifndef _VECTOR_LIST_H
#define _VECTOR_LIST_H

#include "ilist.h"
#include "iarray.h"

class VectorList : virtual public IList, virtual public IArray{
public:
	static const size_t ELEMENT_SIZE = sizeof(Pair);
	static const size_t REALLOC_SIZE = 16;

public:
	VectorList(size_t reallocSize = 0);
	VectorList(VectorList &&other);
	virtual ~VectorList() override;

private:
	size_t		_reallocSize;

	Pair		*_buffer;
	size_t		_bufferReserved;

	uint64_t	_dataCount;
	size_t		_dataSize;

private:
	virtual void _removeAll() override;

	virtual bool _put(const Pair &data) override;
	virtual bool _remove(const char *key) override;

	virtual Pair _getAt(uint64_t index) const override;

	virtual uint64_t _getCount() const override;
	virtual size_t _getSize() const override;

private:
	void _clear(bool alsoFree = false);

	bool _shiftL(uint64_t index);
	bool _shiftR(uint64_t index);

	bool _resize(int delta);

private:
	static size_t __calcNewSize(size_t size, size_t reallocSize);
};

#endif
