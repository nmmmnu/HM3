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

	// no need d-tor,
	// MMAPFile-s will be closed automaticly
	//~DiskTable(){ close(); }

	bool open(const std::string &filename);
	void close();

	operator bool(){
		return _header;
	}

	void printHeader() const{
		_header.print();
	}

public:
	Pair get(const StringRef &key) const;

	Pair getAt(count_type index) const;

	int cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return (count_type) _header.getCount();
	}

	size_t getSize() const{
		return _mmapData.size();
	}

	template <class LOOKUP = IArraySearch::Binary>
	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return LOOKUP::processing(*this, key);
	}

public:
	Iterator begin() const;
	Iterator end() const;

private:
	DiskFileHeader	_header;
	MMAPFile	_mmapIndx;
	MMAPFile	_mmapData;

private:
	const PairBlob *_getAtFromDisk(count_type index) const;

	const PairBlob *_getNextFromDisk(const PairBlob *blob, size_t size = 0) const;

	uint8_t _getVersionFromDisk() const;
};

// ===================================

class DiskTable::Iterator : public IIterator<Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, count_type pos, bool useFastForward);

public:
	Iterator &operator++();
	Iterator &operator--();

	const Pair &operator*() const;

	bool operator==(const Iterator &other) const;

private:
	const DiskTable	&_list;
	count_type	_pos;
	bool		_useFastForward;

private:
	/* !!! */ mutable
	count_type	tmp_pos = 0;

	/* !!! */ mutable
	const PairBlob	*tmp_pod = nullptr;

	/* !!! */ mutable
	Pair		tmp_pair;

};

// ==============================

inline Pair DiskTable::getAt(count_type const index) const{
	return index < getCount() ? _getAtFromDisk(index) : nullptr;
}

inline Pair DiskTable::get(const StringRef &key) const{
	const auto &l = lookup(key);
	return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
}

#endif
