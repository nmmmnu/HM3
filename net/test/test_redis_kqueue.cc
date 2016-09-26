#include "selector/kqueueselector.h"
#include "worker/keyvalueworker.h"
#include "protocol/redisprotocol.h"

using MySelector	= net::selector::KQueueSelector;
using MyProtocol	= net::protocol::RedisProtocol;
using MyWorker		= net::worker::KeyValueWorker<MyProtocol>;

#include "test_selector_impl.h"

