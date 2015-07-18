#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

class DiskTable : virtual public IArray{
public:
	DiskTable(){};
	DiskTable(DiskTable &&other);

	virtual ~DiskTable() override{
		close();
	}

	bool open(const char *filename);
	void close();


private:
	virtual Pair _getAt(uint64_t index) const override;

	virtual uint64_t _getCount() const override;
	virtual size_t _getSize() const override;

private:
	int		_fd;
	const void	*_mem		= NULL;
	uint64_t	_size		= 0;

	uint64_t	_datacount	= 0;

private:
	uint64_t _getCountFromDisk() const;
};

#endif
