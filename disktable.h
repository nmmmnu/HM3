#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarraysearch.h"
#include "iiterator.h"
#include "diskfile.h"
#include "mmapfile.h"

class DiskTable : public IList<DiskTable>{
public:
	class Iterator;

public:
	DiskTable() = default;
	DiskTable(DiskTable &&other) = default;

	~DiskTable(){
		close();
	}

	bool open(const std::string &filename);
	void close();

	void print() const;

public:
	Pair get(const StringRef &key) const;

	Pair getAt(size_t index) const;

	int cmpAt(size_t index, const StringRef &key) const;

	size_t getCount() const{
		return _dataCount;
	}

	size_t getSize() const{
		return _mmapData.size();
	}

	template <class LOOKUP = IArraySearch::Binary>
	std::tuple<int, size_t> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

public:
	Iterator begin() const;
	Iterator end() const;

private:
	MMAPFile	_mmapMeta;
	MMAPFile	_mmapIndx;
	MMAPFile	_mmapData;

	uint8_t		_dataVersion;
	size_t		_dataCount;
	bool		_dataSorted;

	size_t		_tombstones;
	size_t		_createdMin;
	size_t		_createdMax;

private:
	size_t _getCountFromDisk() const;
	bool   _getSortedFromDisk() const;

	size_t _getTombstonesFromDisk() const;
	size_t _getMINCreatedFromDisk() const;
	size_t _getMAXCreatedFromDisk() const;

	const void *_getAtFromDisk(size_t index) const;

	const void *_getNextFromDisk(const void *pod, size_t size = 0) const;

	uint8_t _getVersionFromDisk() const;
};

// ===================================

class DiskTable::Iterator : public IIterator<Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, size_t pos, bool useFastForward);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const DiskTable	&_list;
	size_t		_pos;
	bool		_useFastForward;

private:
	/* !!! */ mutable
	size_t		tmp_pos = 0;

	/* !!! */ mutable
	const void	*tmp_pod = nullptr;

	/* !!! */ mutable
	Pair		tmp_pair;

};

// ==============================

inline Pair DiskTable::getAt(size_t const index) const{
	return index < getCount() ? _getAtFromDisk(index) : nullptr;
}

inline Pair DiskTable::get(const StringRef &key) const{
	const auto &l = lookup(key);
	return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
}

#endif
