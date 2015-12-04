#include "disktable.h"

#include "pairpod.h"

#include "mytime.h"

#include <endian.h>	// htobe16

bool DiskTable::open(const std::string &filename){
	_header.open(filename);

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

int DiskTable::cmpAt(size_t const index, const StringRef &key) const{
	const PairPOD *p = (const PairPOD *) _getAtFromDisk(index);
	// StringRef is not null terminated
	return p ? p->cmp(key.data(), key.size()) : PairPOD::cmpZero();
}

const void *DiskTable::_getAtFromDisk(size_t const index) const{
	const uint64_t *ptr_be = (const uint64_t *) _mmapIndx.safeAccess( index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		const void *pod = _mmapData.safeAccess( offset );
		return pod; // invalid pod is nullptr anyway
	}

	return nullptr;
}

const void *DiskTable::_getNextFromDisk(const void *pod, size_t size) const{
	if (size == 0){
		const PairPOD *p = (const PairPOD *) pod;
		size = p->getSize();
	}

	const char *podc = (const char *) pod;

	//printf("%p %p\n", podc, podc);

	const void *podNext = _mmapData.safeAccess( podc + size );
	return podNext; // invalid pod is nullptr anyway
}

// ===================================

DiskTable::Iterator::Iterator(const DiskTable &list, size_t const pos, bool const useFastForward) :
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


