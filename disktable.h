#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

#include "stringref.h"

class DiskTable : virtual public IArray{
public:
	DiskTable() = default;

	DiskTable(DiskTable &&other);

	virtual ~DiskTable() override{
		close();
	}
	
	bool open(const StringRef &filename);

	void close();


private:
	virtual Pair _getAt(uint64_t index) const override;
	virtual int  _cmpAt(uint64_t index, const StringRef &key) const override;

	virtual count_type _getCount() const override;
	virtual size_t _getSize() const override;

private:
	const void	*_mem		= nullptr;
	uint64_t	_size		= 0;

	int		_fd;

	count_type	_datacount	= 0;

private:
	uint64_t _getCountFromDisk() const;
	
	const void *_getAtFromDisk(uint64_t index) const;
};

// ==============================

inline Pair DiskTable::_getAt(uint64_t const index) const{
	return _getAtFromDisk(index);
}

inline DiskTable::count_type DiskTable::_getCount() const{
	return _datacount;
}

#endif
