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

	virtual uint64_t getCount() const override{
		return _datacount;
	};

	virtual size_t getSize() const override;

protected:
	virtual const void *_getAt(uint64_t index) const override;

private:
	int		_fd;
	const void	*_mem		= NULL;
	uint64_t	_size		= 0;

	uint64_t	_datacount	= 0;

private:
	uint64_t _getCount() const;
};

#endif
