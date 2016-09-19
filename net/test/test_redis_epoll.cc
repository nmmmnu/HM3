#include "selector/epollselector.h"
#include "worker/keyvalueworker.h"
#include "protocol/redisprotocol.h"

using MySelector	= net::selector::EPollSelector;
using MyProtocol	= net::protocol::RedisProtocol;
using MyWorker		= net::worker::KeyValueWorker<MyProtocol>;

#include "test_selector_impl.h"

