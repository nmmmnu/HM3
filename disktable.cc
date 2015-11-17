#include "disktable.h"

#include "pairpod.h"

#include <endian.h>	// htobe16

int DiskTable::cmpAt(size_t const index, const StringRef &key) const{
	const PairPOD *p = (const PairPOD *) _getAtFromDisk(index);
	// StringRef is not null terminated
	return p->cmp(key.data(), key.size());
}

size_t DiskTable::getSize() const{
	return _mmapData.size();
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

// ===================================

DiskTable::Iterator::Iterator(const DiskTable &list, size_t const pos) :
			_list(list),
			_pos(pos){}

DiskTable::Iterator &DiskTable::Iterator::operator++(){
	++_pos;
	return *this;
}

DiskTable::Iterator &DiskTable::Iterator::operator--(){
	--_pos;
	return *this;
}

const Pair &DiskTable::Iterator::operator*() const{
	tmp_pairPlaceholderDeref = _list.getAt(_pos);

	return tmp_pairPlaceholderDeref;
}

bool DiskTable::Iterator::operator==(const Iterator &other) const{
	return &_list == &other._list && _pos == other._pos;
}

// ===================================

auto DiskTable::begin() const -> Iterator{
	return Iterator(*this, 0);
}

auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount());
}


