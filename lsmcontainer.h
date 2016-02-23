#ifndef _LSM_CONTAINER_H
#define _LSM_CONTAINER_H

#include <vector>

#include "disktable.h"

namespace LSMContainer{
	using container_type	= std::vector<DiskTable>;
};

#endif

