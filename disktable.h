#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

#include "stringref.h"

class DiskTable : virtual public IArray{
public:
	DiskTable() = default;
	DiskTable(DiskTable &&other);
	~DiskTable() override{
		close();
	}

	bool open(const StringRef &filename);

	void close();


private:
	Pair _getAt(uint64_t index) const override;
	int  _cmpAt(uint64_t index, const StringRef &key) const override;

	uint64_t _getCount() const override{
		return _dataCount;
	}

	size_t _getSize() const override;

private:
	const void	*_mem		= nullptr;
	uint64_t	_size		= 0;

	int		_fd;

	uint64_t	_dataCount	= 0;

private:
	uint64_t _getCountFromDisk() const;

	const void *_getAtFromDisk(uint64_t index) const;
};

// ==============================

inline Pair DiskTable::_getAt(uint64_t const index) const{
	return index < _dataCount ? _getAtFromDisk(index) : nullptr;
}

#endif
