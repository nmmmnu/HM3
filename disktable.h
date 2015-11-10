#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarraysearch.h"
#include "iiterator.h"

class DiskTable : public IList<DiskTable>{
public:
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
	Pair get(const StringRef &key) const;

	Pair getAt(count_type index) const;

	int cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return _dataCount;
	}

	size_t getSize() const;

	template <class LOOKUP = IArraySearch::Binary>
	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

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

class DiskTable::Iterator : public IIterator<Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, count_type const pos);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const DiskTable	&_list;
	count_type	_pos;

private:
	/* !!! */ mutable
	Pair		tmp_pairPlaceholderDeref;

};

// ==============================

inline Pair DiskTable::getAt(count_type index) const{
	return index < getCount() ? _getAtFromDisk(index) : nullptr;
}

inline Pair DiskTable::get(const StringRef &key) const{
	const auto &l = lookup(key);
	return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
}

#endif
