#include "diskfileheader.h"
#include "mytime.h"

#include <fstream>

#include <inttypes.h>	// PRIu64

DiskFileHeader::DiskFileHeader(uint64_t const count,
					bool const sorted,
					uint64_t const tombstones, uint64_t const createdMin, uint64_t const createdMax):
				_version(VERSION),
				_count(count),
				_created(MyTime::now()),
				_sorted(sorted),
				_tombstones(tombstones),
				_createdMin(createdMin),
				_createdMax(createdMax){}

bool DiskFileHeader::open(const StringRef &filename){
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	return open(file);
}

bool DiskFileHeader::open(std::ifstream &file_meta){
	POD pod;
	memset(&pod, 0, sizeof(POD));

	file_meta.read( (char *) &pod, sizeof(PODBase));

	if (! file_meta)
		return false;

	// check the logo
	if (strncmp(pod.logo, TITLE, TITLE_LOGO_SIZE) != 0)
		return false;

	_version	= pod.version;
	_count		= (size_t) be64toh(pod.size);

	// read the rest, without rewind :-)
	size_t additionalSize = sizeof(POD) - sizeof(PODBase);

	file_meta.read( ((char *) &pod) + sizeof(PODBase), additionalSize);

	if (! file_meta && file_meta.gcount() < (std::streamsize) additionalSize){
		_clear();
		return true;
	}

	_created	= be64toh(pod.created);

	_sorted		= pod.sorted == HEADER_SORTED ? true : false;

	_tombstones	= (size_t) be64toh(pod.tombstones);

	_createdMin	= be64toh(pod.createdMin);
	_createdMax	= be64toh(pod.createdMax);

	return true;
}

auto DiskFileHeader::_createPOD() const -> POD{
	POD pod;

	memset(&pod, 0, sizeof(POD));

	strcpy(pod.logo, TITLE);

	pod.version	= VERSION;
	pod.size	= htobe64(_count);

	pod.created	= htobe64(_created);
	pod.sorted	= _sorted ? HEADER_SORTED : HEADER_NOT_SORTED;
	pod.tombstones	= htobe64(_tombstones);
	pod.createdMin	= htobe64(_createdMin);
	pod.createdMax	= htobe64(_createdMax);

	return pod;
}

bool DiskFileHeader::writeToFile(std::ofstream &file) const{
	const POD pod = _createPOD();

	file.write( (const char *) & pod, sizeof(POD));

	return true;
}

void DiskFileHeader::_clear(){
	_created	= 0;
	_sorted		= false;
	_tombstones	= 0;
	_createdMin	= 0;
	_createdMax	= 0;
}

void DiskFileHeader::print() const{
	printf("%-14s: %u\n",		"Version",	_version);
	printf("%-14s: %zu\n",	"Records",	_count); // PRIu64
	printf("%-14s: %s\n",		"Created",	_created ? MyTime::toString(_created) : "n/a");
	printf("%-14s: %s\n",		"Sorted",	_sorted ? "Yes" : "No");

	printf("%-14s: %zu\n",	"Tombstones",	_tombstones); // PRIu64
	printf("%-14s: %s\n",		"Created::MIN",	_createdMin ? MyTime::toString(_createdMin) : "n/a");
	printf("%-14s: %s\n",		"Created::MAX",	_createdMax ? MyTime::toString(_createdMax) : "n/a");
}
