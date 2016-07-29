#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "iobuffer.h"

#include "timer.h"

#include <cstdlib>

namespace net{


class ClientBuffer :
		public IOBuffer,
		public Timer_{
};


} // namespace

#endif

