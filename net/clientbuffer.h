#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "iobuffer.h"

#include "timer.h"

#include <cstdlib>

namespace net{


template<size_t CAPACITY>
class ClientBuffer :
		public IOBuffer<CAPACITY>,
		public Timer_{
};


} // namespace

#endif

