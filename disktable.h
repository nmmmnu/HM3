#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"
#include "iiterator.h"

class DiskTable : public IList<DiskTable>, public IArray<DiskTable, IArraySearch::Binary>{
public:
	typedef IListDefs::count_type count_type;

	class Iterator;

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

public:
	Iterator begin() const;
	Iterator end() const;

private:
	const void	*_mem		= nullptr;
	uint64_t	_size		= 0;

	int		_fd;

	uint64_t	_dataCount	= 0;

private:
	uint64_t _getCountFromDisk() const;

	const void *_getAtFromDisk(count_type index) const;
};

// ===================================

class DiskTable::Iterator : public IIterator<DiskTable::Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, count_type const pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	Pair operator*() const;

	bool operator==(const Iterator &other) const;

private:
	// disabled method...
	Pair *operator->() const;

private:
	const DiskTable		&_list;
	count_type		_pos;
};

// ==============================

inline Pair DiskTable::getAt(count_type index) const{
	return index < getCount() ? _getAtFromDisk(index) : nullptr;
}

#endif
