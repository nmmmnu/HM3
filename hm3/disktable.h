#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "diskfile/diskfile.h"
#include "mmapfile.h"

#include "arraysearch/binary.h"
#include "iiterator.h"
#include "ilist.h"

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

public:
	Iterator getIterator(const StringRef &key) const;
	Iterator begin() const;
	Iterator end() const;

private:
	const PairBlob *validateFromDisk_(const PairBlob *blob) const;

	const PairBlob *getAtFromDisk_(count_type index) const;

	const PairBlob *getNextFromDisk_(const PairBlob *blob, size_t size = 0) const;

	uint8_t getVersionFromDisk_() const;

private:
	diskfile::DiskFileHeader	header_;
	MMAPFile	mmapIndx_;
	MMAPFile	mmapData_;

	bool		validate_;

	ArrayLookup	lookup_;

private:
	auto lookup(const StringRef &key) const -> decltype( lookup_(*this, key) ){
		return lookup_(*this, key);
	}
};

// ===================================

class DiskTable::Iterator : public IIterator<Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, count_type pos, bool useFastForward);

public:
	Iterator &operator++(){
		++pos_;
		return *this;
	}

	Iterator &operator--(){
		--pos_;
		return *this;
	}

	bool operator==(const Iterator &other) const{
		return &list_ == &other.list_ && pos_ == other.pos_;
	}

	const Pair &operator*() const;

private:
	const DiskTable	&list_;
	count_type	pos_;
	bool		useFastForward_;

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
	const auto &lr = lookup_(*this, key);
	return lr ? getAt( lr.get() ) : nullptr;
}

// ==============================

inline auto DiskTable::getIterator(const StringRef &key) const -> Iterator{
	const auto &lr = lookup_(*this, key);
	return Iterator(*this, lr.get(), header_.getSorted());
}

inline auto DiskTable::begin() const -> Iterator{
	return Iterator(*this, 0, header_.getSorted());
}

inline auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}


} // namespace

#endif
