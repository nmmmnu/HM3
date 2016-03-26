#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "arraysearch.h"
#include "iiterator.h"
#include "diskfile.h"
#include "mmapfile.h"


namespace hm3{


class DiskTable : public IList<DiskTable>{
private:
	using ArrayLookup	= arraysearch::Binary;

public:
	class Iterator;

public:
	DiskTable(bool validate = true) : validate_(validate){}

	DiskTable(DiskTable &&other) = default;

	// no need d-tor,
	// MMAPFile-s will be closed automaticly
	//~DiskTable(){ close(); }

	bool open(const std::string &filename);
	void close();

	operator bool(){
		return header_;
	}

	void printHeader() const{
		header_.print();
	}

public:
	Pair get(const StringRef &key) const;

	Pair getAt(count_type index) const;

	int cmpAt(count_type index, const StringRef &key) const;

	count_type getCount() const{
		return (count_type) header_.getCount();
	}

	size_t getSize() const{
		return mmapData_.size();
	}

	std::tuple<int, count_type> lookup(const StringRef &key) const{
		return lookup_(*this, key);
	}

public:
	Iterator begin() const;
	Iterator end() const;

private:
	const PairBlob *validateFromDisk_(const PairBlob *blob) const;

	const PairBlob *getAtFromDisk_(count_type index) const;

	const PairBlob *getNextFromDisk_(const PairBlob *blob, size_t size = 0) const;

	uint8_t getVersionFromDisk_() const;

private:
	DiskFileHeader	header_;
	MMAPFile	mmapIndx_;
	MMAPFile	mmapData_;

	bool		validate_;

	ArrayLookup	lookup_;
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
	return index < getCount() ? getAtFromDisk_(index) : nullptr;
}

inline Pair DiskTable::get(const StringRef &key) const{
	const auto &l = lookup(key);
	return std::get<0>(l) ? nullptr : getAt( std::get<1>(l) );
}


} // namespace

#endif
