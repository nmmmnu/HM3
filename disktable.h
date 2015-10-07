#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

class DiskTable : public IArray<DiskTable>{
public:
	DiskTable() = default;
	DiskTable(DiskTable &&other);
	~DiskTable(){
		close();
	}

	bool open(const StringRef &filename);

	void close();


public:
	Pair getAt(count_type index) const;
	int  cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return _dataCount;
	}

	size_t getSize() const;

private:
	const void	*_mem		= nullptr;
	uint64_t	_size		= 0;

	int		_fd;

	uint64_t	_dataCount	= 0;

private:
	uint64_t _getCountFromDisk() const;

	const void *_getAtFromDisk(count_type index) const;
};

// ==============================

inline Pair DiskTable::getAt(count_type const index) const{
	return index < getCount() ? _getAtFromDisk(index) : nullptr;
}

#endif
