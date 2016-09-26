#include "diskfileheader.h"
#include "mytime.h"

#include <endian.h>

#include <fstream>

#include <inttypes.h>	// PRIu64

namespace hm3{
namespace diskfile{


DiskFileHeader::DiskFileHeader(uint64_t const count,
					bool const sorted,
					uint64_t const tombstones, uint64_t const createdMin, uint64_t const createdMax):
				version_(VERSION),
				count_(count),
				created_(MyTime::now()),
				sorted_(sorted),
				tombstones_(tombstones),
				createdMin_(createdMin),
				createdMax_(createdMax){}

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

	version_	= pod.version;
	count_		= be64toh(pod.size);

	// read the rest, without rewind :-)
	size_t additionalSize = sizeof(POD) - sizeof(PODBase);

	file_meta.read( ((char *) &pod) + sizeof(PODBase), (std::streamsize) additionalSize);

	if (! file_meta && file_meta.gcount() < (std::streamsize) additionalSize){
		clear_();
		return true;
	}

	created_	= be64toh(pod.created);

	sorted_		= pod.sorted == HEADER_SORTED ? true : false;

	tombstones_	= be64toh(pod.tombstones);

	createdMin_	= be64toh(pod.createdMin);
	createdMax_	= be64toh(pod.createdMax);

	return true;
}

auto DiskFileHeader::createPOD_() const -> POD{
	POD pod;

	memset(&pod, 0, sizeof(POD));

	strcpy(pod.logo, TITLE);

	pod.version	= VERSION;
	pod.size	= htobe64(count_);

	pod.created	= htobe64(created_);
	pod.sorted	= sorted_ ? HEADER_SORTED : HEADER_NOT_SORTED;
	pod.tombstones	= htobe64(tombstones_);
	pod.createdMin	= htobe64(createdMin_);
	pod.createdMax	= htobe64(createdMax_);

	return pod;
}

bool DiskFileHeader::writeToFile(std::ofstream &file) const{
	const POD pod = createPOD_();

	file.write( (const char *) & pod, sizeof(POD));

	return true;
}

void DiskFileHeader::clear_(){
	created_	= 0;
	sorted_		= false;
	tombstones_	= 0;
	createdMin_	= 0;
	createdMax_	= 0;
}

void DiskFileHeader::print() const{
	printf("%-14s: %u\n",		"Version",	version_);
	printf("%-14s: %" PRIu64 "\n",	"Records",	count_); // PRIu64
	printf("%-14s: %s\n",		"Created",	created_ ? MyTime::toString(created_) : "n/a");
	printf("%-14s: %s\n",		"Sorted",	sorted_ ? "Yes" : "No");

	printf("%-14s: %" PRIu64 "\n",	"Tombstones",	tombstones_); // PRIu64
	printf("%-14s: %s\n",		"Created::MIN",	createdMin_ ? MyTime::toString(createdMin_) : "n/a");
	printf("%-14s: %s\n",		"Created::MAX",	createdMax_ ? MyTime::toString(createdMax_) : "n/a");
}


} // namespace diskfile
} // namespace

