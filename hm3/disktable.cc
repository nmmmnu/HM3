#include "disktable.h"

#include "pairblob.h"

#include "binarysearch.h"

#include <endian.h>	// htobe16


namespace hm3{

bool DiskTable::open(const std::string &filename){
	using DiskFile = diskfile::DiskFile;

	header_.open(DiskFile::filenameMeta(filename));

	if (header_ == false)
		return false;

	mmapIndx_.open(DiskFile::filenameIndx(filename));
	mmapData_.open(DiskFile::filenameData(filename));

	return true;
}

void DiskTable::close(){
	mmapIndx_.close();
	mmapData_.close();
}

inline bool DiskTable::binarySearch_(const StringRef &key, size_type &result) const{
	return binarySearch(*this, getMemSize(), key, BinarySearchCompList{}, result);
}

Pair DiskTable::get(const StringRef &key) const{
	size_type result;
	bool const found = binarySearch_(key, result);

	return found ? getAt( result ) : nullptr;
}

auto DiskTable::lowerBound(const StringRef &key) const -> Iterator{
	size_type result;
	/* bool const found = */ binarySearch_(key, result);

	return Iterator(*this, result, header_.getSorted());
}

int DiskTable::cmpAt(size_type const index, const StringRef &key) const{
	const PairBlob *p = getAtFromDisk_(index);
	// StringRef is not null terminated
	return p ? p->cmp(key.data(), key.size()) : Pair::CMP_ZERO;
}

const PairBlob *DiskTable::validateFromDisk_(const PairBlob *blob) const{
	if (blob == nullptr)
		return nullptr;

	if (! validate_)
		return blob;

	return blob->validChecksum() ? blob : nullptr;
}

size_t DiskTable::getAtOffset(size_type const index) const{
	const uint64_t *ptr_be = (const uint64_t *) mmapIndx_.safeAccess( (size_t) index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		return offset;
	}

	return (size_t) -1;
}


const PairBlob *DiskTable::getAtFromDisk_(size_type const index) const{
	const uint64_t *ptr_be = (const uint64_t *) mmapIndx_.safeAccess( (size_t) index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		const PairBlob *blob = (const PairBlob *) mmapData_.safeAccess( offset );
		return validateFromDisk_(blob);
	}

	return nullptr;
}

const PairBlob *DiskTable::getNextFromDisk_(const PairBlob *blob, size_t size) const{
	if (size == 0)
		size = blob->getMemSize();

	const char *blobc = (const char *) blob;

	const PairBlob *blobNext = (const PairBlob *) mmapData_.safeAccess( blobc + size );
	return validateFromDisk_(blobNext);
}

// ===================================

DiskTable::Iterator::Iterator(const DiskTable &list, size_type const pos, bool const useFastForward) :
			list_(list),
			pos_(pos),
			useFastForward_(useFastForward){}

const Pair &DiskTable::Iterator::operator*() const{
	if (tmp_pod && tmp_pos == pos_)
		return tmp_pair;

	if (useFastForward_ && tmp_pod && tmp_pos == pos_ - 1){
		// get data without seek, walk forward
		// this gives 50% performance
		tmp_pod = list_.getNextFromDisk_(tmp_pod, tmp_pair.getMemSize() );
	}else{
		// get data via seek
		tmp_pod = list_.getAtFromDisk_(pos_);
	}

	tmp_pos = pos_;

	tmp_pair = tmp_pod;

	return tmp_pair;
}


} // namespace

