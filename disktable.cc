#include "disktable.h"

#include "pairblob.h"

#include <endian.h>	// htobe16


namespace hm3{


bool DiskTable::open(const std::string &filename){
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

int DiskTable::cmpAt(count_type const index, const StringRef &key) const{
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

const PairBlob *DiskTable::getAtFromDisk_(count_type const index) const{
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
		size = blob->getSize();

	const char *blobc = (const char *) blob;

	const PairBlob *blobNext = (const PairBlob *) mmapData_.safeAccess( blobc + size );
	return validateFromDisk_(blobNext);
}

// ===================================

DiskTable::Iterator::Iterator(const DiskTable &list, count_type const pos, bool const useFastForward) :
			_list(list),
			_pos(pos),
			_useFastForward(useFastForward){}

DiskTable::Iterator &DiskTable::Iterator::operator++(){
	++_pos;
	return *this;
}

DiskTable::Iterator &DiskTable::Iterator::operator--(){
	--_pos;
	return *this;
}

const Pair &DiskTable::Iterator::operator*() const{
	if (tmp_pod && tmp_pos == _pos)
		return tmp_pair;

	if (_useFastForward && tmp_pod && tmp_pos == _pos - 1){
		// get data without seek, walk forward
		// this gives 50% performance
		tmp_pod = _list.getNextFromDisk_(tmp_pod, tmp_pair.getSize() );
	}else{
		// get data via seek
		tmp_pod = _list.getAtFromDisk_(_pos);
	}

	tmp_pos = _pos;

	tmp_pair = tmp_pod;

	return tmp_pair;
}

bool DiskTable::Iterator::operator==(const Iterator &other) const{
	return &_list == &other._list && _pos == other._pos;
}

// ===================================

auto DiskTable::begin() const -> Iterator{
	return Iterator(*this, 0, header_.getSorted());
}

auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}


} // namespace

