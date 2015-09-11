#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"

#include <string>

class DiskTable : virtual public IArray{
public:
	DiskTable() = default;

	DiskTable(DiskTable &&other);

	virtual ~DiskTable() override{
		close();
	}
	
	bool open(const std::string &filename);

	void close();


private:
	virtual Pair _getAt(uint64_t index) const override;
	virtual int _cmpAt(uint64_t index, const std::string &key) const override;
	virtual int _cmpAt(uint64_t index, const char *key) const override;

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

inline int DiskTable::_cmpAt(uint64_t const index, const std::string &key) const{
	return _cmpAt(index, key.c_str());
}

#endif
