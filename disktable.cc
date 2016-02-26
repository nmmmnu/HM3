#include "disktable.h"

#include "pairblob.h"

#include <endian.h>	// htobe16

bool DiskTable::open(const std::string &filename){
	_header.open(DiskFile::filenameMeta(filename));

	if (_header == false)
		return false;

	_mmapIndx.open(DiskFile::filenameIndx(filename));
	_mmapData.open(DiskFile::filenameData(filename));

	return true;
}

void DiskTable::close(){
	_mmapIndx.close();
	_mmapData.close();
}

int DiskTable::cmpAt(count_type const index, const StringRef &key) const{
	const PairBlob *p = _getAtFromDisk(index);
	// StringRef is not null terminated
	return p ? p->cmp(key.data(), key.size()) : Pair::CMP_ZERO;
}

const PairBlob *DiskTable::_validateFromDisk(const PairBlob *blob) const{
	if (! _validate)
		return blob;

	return blob->validChecksum() ? blob : nullptr;
}

const PairBlob *DiskTable::_getAtFromDisk(count_type const index) const{
	const uint64_t *ptr_be = (const uint64_t *) _mmapIndx.safeAccess( index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		const PairBlob *blob = (const PairBlob *) _mmapData.safeAccess( offset );
		return _validateFromDisk(blob);
	}

	return nullptr;
}

const PairBlob *DiskTable::_getNextFromDisk(const PairBlob *blob, size_t size) const{
	if (size == 0)
		size = blob->getSize();

	const char *blobc = (const char *) blob;

	const PairBlob *blobNext = (const PairBlob *) _mmapData.safeAccess( blobc + size );
	return _validateFromDisk(blobNext);
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
		tmp_pod = _list._getNextFromDisk(tmp_pod, tmp_pair.getSize() );
	}else{
		// get data via seek
		tmp_pod = _list._getAtFromDisk(_pos);
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
	return Iterator(*this, 0, _header.getSorted());
}

auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}

