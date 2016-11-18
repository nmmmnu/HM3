#include "worker/keyvalueworker.h"
#include "protocol/redisprotocol.h"

struct KeyValueMockAdapter{
	// Mock commands
	std::string info() const{
		return "Mock Adapter\n";
	}

	std::string get(const StringRef &key) const{
		return "value";
	}

	std::vector<StringRef> getall(const StringRef &key) const{
		return {
			"key1", "value1",
			"key2", "value2",
			"key3", "value3",
			"key4", "value4"
		};
	}

	bool refresh(){
		return true;
	}
};

using MyProtocol	= net::protocol::RedisProtocol;
using MyWorker		= net::worker::KeyValueWorker<MyProtocol, KeyValueMockAdapter>;

MyWorker myWorkerFactory(){
	static KeyValueMockAdapter adapter;

	return { adapter };
}

#include "test_selector_impl.h"

