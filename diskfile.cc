#include "diskfile.h"
#include "mytime.h"

auto DiskFileHeader::create(size_t const datacount, size_t const tombstones,
				uint64_t const createdMin, uint64_t const createdMax) -> POD{
	POD pod;

	memset(&pod, 0, sizeof(POD));

	strcpy(pod.logo, TITLE);

	pod.version	= VERSION;
	pod.size	= htobe64(datacount);

	pod.created	= htobe64(MyTime::now());
	pod.sorted	= HEADER_SORTED;
	pod.tombstones	= htobe64(tombstones);
	pod.createdMin	= htobe64(createdMin);
	pod.createdMax	= htobe64(createdMax);

	return pod;
}

