#include "disktable.h"

#include "pairpod.h"

#include <endian.h>	// htobe16

int DiskTable::cmpAt(size_t const index, const StringRef &key) const{
	const PairPOD *p = (const PairPOD *) _getAtFromDisk(index);
	// StringRef is not null terminated
	return p->cmp(key.data(), key.size());
}

uint64_t DiskTable::_getCountFromDisk() const{
	const DiskTableHeader *head = (const DiskTableHeader *) _mmapMeta.mem();
	return be64toh(head->size);
}

const void *DiskTable::_getAtFromDisk(size_t const index) const{
	// index is checked in the parent

	// TODO: check if we are inside the memory block.

	const uint64_t *mem_index = (const uint64_t *) _mmapIndx.mem();

	const uint64_t ptr = be64toh( mem_index[index] );

	const char *mem = (const char *) _mmapData.mem();
	return & mem[ptr];
}

const void *DiskTable::_getNextFromDisk(const void *pod, size_t podSize) const{
	const char *mem = (const char *) pod;

	if (podSize == 0){
		const PairPOD *p = (const PairPOD *) pod;
		podSize = p->getSize();
	}

	return & mem[podSize];
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

auto DiskTable::begin(bool const useFastForward) const -> Iterator{
	return Iterator(*this, 0, useFastForward);
}

auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}


