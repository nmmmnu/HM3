#include "disktable.h"

#include "pairpod.h"

#include "mytime.h"

#include <endian.h>	// htobe16

bool DiskTable::open(const std::string &filename){
	_mmapMeta.open(filename);
	_mmapIndx.open(DiskFile::filenameIndx(filename));
	_mmapData.open(DiskFile::filenameData(filename));

	_dataCount	= _getCountFromDisk();
	_dataSorted	= _getSortedFromDisk();
	_dataVersion	= _getVersionFromDisk();

	_tombstones	= _getTombstonesFromDisk();
	_createdMin	= _getMINCreatedFromDisk();
	_createdMax	= _getMAXCreatedFromDisk();

	if (! _dataVersion){
		close();

		return false;
	}

	return true;
}

void DiskTable::close(){
	_mmapMeta.close();
	_mmapIndx.close();
	_mmapData.close();
}

void DiskTable::print() const{
	printf("%-12s: %u\n",	"Version",	_dataVersion);
	printf("%-12s: %zu\n",	"Records",	_dataCount);
	printf("%-12s: %s\n",	"Sorted",	_dataSorted ? "Yes" : "No");

	printf("%-12s: %zu\n",	"Tombstones",	_tombstones);
	printf("%-12s: %s\n",	"Created::MIN",	MyTime::toString(_createdMin));
	printf("%-12s: %s\n",	"Created::MAX",	MyTime::toString(_createdMax));
}

int DiskTable::cmpAt(size_t const index, const StringRef &key) const{
	const PairPOD *p = (const PairPOD *) _getAtFromDisk(index);
	// StringRef is not null terminated
	return p ? p->cmp(key.data(), key.size()) : PairPOD::cmpZero();
}

size_t DiskTable::_getCountFromDisk() const{
	const uint64_t *size = (const uint64_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, size) );

	return size ? (size_t) be64toh(*size) : 0;
}

size_t DiskTable::_getTombstonesFromDisk() const{
	const uint64_t *size = (const uint64_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, tombstones) );

	return size ? (size_t) be64toh(*size) : 0;
}

size_t DiskTable::_getMINCreatedFromDisk() const{
	const uint64_t *size = (const uint64_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, createdMin) );

	return size ? (size_t) be64toh(*size) : 0;
}

size_t DiskTable::_getMAXCreatedFromDisk() const{
	const uint64_t *size = (const uint64_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, createdMax) );

	return size ? (size_t) be64toh(*size) : 0;
}

bool DiskTable::_getSortedFromDisk() const{
	const uint8_t *sorted = (const uint8_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, sorted) );

	//printf("%u\n", *sorted);

	if (sorted){
		switch(*sorted){
		case DiskFile::NOT_SORTED:	return false;
		case DiskFile::SORTED:		return true;
		}
	}

	return false;
}

uint8_t DiskTable::_getVersionFromDisk() const{
	const uint8_t *version = (const uint8_t *) _mmapMeta.safeAccess( offsetof(DiskTableHeader, version) );

	if (version){
		const char *logo = (const char *) _mmapMeta.mem();

		if (strncmp(logo, DISK_TABLE_LOGO, 4) != 0)
			return 0;

		return *version;
	}

	return 0;
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
	return Iterator(*this, 0, _dataSorted);
}

auto DiskTable::end() const -> Iterator{
	return Iterator(*this, getCount(), false);
}


