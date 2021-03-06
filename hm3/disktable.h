#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "diskfile/diskfileheader.h"
#include "mmapfile.h"
#include "blobref.h"

#include "ilist.h"

#include "iiterator.h"

namespace hm3{


class DiskTable : public List<DiskTable>{
public:
	class Iterator;

private:
	static constexpr size_type	BIN_SEARCH_MINIMUM_DISTANCE	= 3;

public:
	DiskTable(bool validate = true) :
			//	mmapIndx_(MADV_RANDOM),
				mmapData_(MADV_RANDOM),
			//	mmapTree_(MADV_RANDOM),
			//	mmapKeys_(MADV_RANDOM),
				validate_(validate){}

	DiskTable(DiskTable &&other) = default;

	// no need d-tor,
	// MMAPFile-s will be closed automatically
	~DiskTable() = default;

	bool open(const StringRef &filename);
	void close();

	operator bool(){
		return header_;
	}

	void printHeader() const{
		header_.print();
	}

public:
	Pair get(const StringRef &key) const;

	Pair getAt(size_type index) const{
		return index < getCount() ? getAtFromDisk_(index) : nullptr;
	}

	int cmpAt(size_type index, const StringRef &key) const;

	size_type getCount() const{
		return static_cast<size_type>(header_.getCount());
	}

	size_t getBytes() const{
		return mmapData_.size();
	}

public:
	Iterator lowerBound(const StringRef &key) const;
	Iterator begin() const;
	Iterator end() const;

public:
//	size_t getAtOffset(size_type index) const;

private:
	const PairBlob *validateFromDisk_(const PairBlob *blob) const;

	const PairBlob *getAtFromDisk_(size_type index) const;

	const PairBlob *getNextFromDisk_(const PairBlob *blob, size_t size = 0) const;

	uint8_t getVersionFromDisk_() const;

private:
	bool binarySearch_(const StringRef &key, size_type &result) const;

	bool btreeSearch_(const StringRef &key, size_type &result) const;

	bool search_(const StringRef &key, size_type &result) const;

	static void  openFile__(MMAPFile &file, BlobRef &blob, const StringRef &filename);
	static void closeFile__(MMAPFile &file, BlobRef &blob);

private:
	diskfile::DiskFileHeader	header_;
	MMAPFile			mmapIndx_;
	MMAPFile			mmapData_;

	MMAPFile			mmapTree_;
	MMAPFile			mmapKeys_;

	bool				validate_;

	BlobRef				blobIndx_;
	BlobRef				blobData_;

	BlobRef				blobTree_;
	BlobRef				blobKeys_;
};

// ===================================

class DiskTable::Iterator : public IIterator<Iterator>{
private:
	friend class DiskTable;
	Iterator(const DiskTable &list, size_type pos, bool useFastForward);

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
	size_type	pos_;
	bool		useFastForward_;

private:
	/* !!! */ mutable
	size_type	tmp_pos = 0;

	/* !!! */ mutable
	const PairBlob	*tmp_pod = nullptr;

	/* !!! */ mutable
	Pair		tmp_pair;

};

// ==============================

inline auto DiskTable::begin() const -> Iterator{
	return Iterator(*this, 0, header_.getSorted());
}

inline auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}


} // namespace

#endif
