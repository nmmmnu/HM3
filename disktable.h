#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

class DiskTable : virtual public IArray{
public:
	DiskTable() = default;

	DiskTable(DiskTable &&other);

	virtual ~DiskTable() override{
		close();
	}

	bool open(const char *filename);
	void close();


private:
	virtual Pair _getAt(uint64_t index) const override;
	virtual int _cmpAt(uint64_t index, const char *key) const override;

	virtual uint64_t _getCount() const override;
	virtual size_t _getSize() const override;

private:
	const void	*_mem		= nullptr;
	uint64_t	_size		= 0;

	int		_fd;

	uint64_t	_datacount	= 0;

private:
	uint64_t _getCountFromDisk() const;

	const PairPOD *_getAtFromDisk(uint64_t index) const;
};

#endif
