
#include <cstring>

#include "protocol/protocoldefs.h"


namespace net{
namespace worker{


template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::operator()(CONNECTION &buffer){
	if (buffer.size() == 0)
		return WorkerStatus::PASS;

	using Status = net::protocol::ProtocolStatus;

	const Status status = protocol_( StringRef{ buffer.data(), buffer.size() } );

	switch(status){
		case Status::BUFFER_NOT_READ :
			return WorkerStatus::PASS;

		case Status::OK :
			return process_request_(buffer);

		default: // for GCC warning...
		case Status::ERROR :
			return sendResponseError_(buffer, "Internal Error");
	}

	// must not get here
	//return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::process_request_(CONNECTION &buffer){
	protocol_.print();

	const auto &p = protocol_.getParams();

	if (p[0] == SHUTDOWN || p[0] == SHUTDOWN2){
		return WorkerStatus::SHUTDOWN;
	}

	if (p[0] == EXIT || p[0] == EXIT2){
		return WorkerStatus::DISCONNECT;
	}

	if (p[0] == RELOAD || p[0] == RELOAD2){
		return sendResponseString_(buffer, "OK");
	}

	if (p[0] == INFO || p[0] == INFO2){
		if (p.size() != 1)
			return sendResponseError_(buffer, "Bad request");

		return sendResponseString_(buffer, db_info_(db_) );
	}

	if (p[0] == GET || p[0] == GET2){
		if (p.size() != 2)
			return sendResponseError_(buffer, "Bad request");

		const auto &key = p[1];

		return sendResponseString_(buffer, db_get_(db_, key));
	}

	if (p[0] == GETALL || p[0] == GETALL2){
		if (p.size() != 2)
			return sendResponseError_(buffer, "Bad request");

		const auto &key = p[1];
		(void) key;

		return sendResponseMulti_(buffer, db_getall_(db_, key) );
	}

	return sendResponseError_(buffer, "Not Implemented");
}

// ====================================

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseError_(CONNECTION &buffer, const StringRef &msg){
	buffer.clear();
	protocol_.response_error(buffer, msg);
	return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseString_(CONNECTION &buffer, const StringRef &msg){
	buffer.clear();
	protocol_.response_string(buffer, msg);
	return WorkerStatus::WRITE;
}

template<class PROTOCOL, class DB_ADAPTER>
template<class CONNECTION, class CONTAINER>
WorkerStatus KeyValueWorker<PROTOCOL, DB_ADAPTER>::sendResponseMulti_(CONNECTION &buffer, const CONTAINER &msg){
	buffer.clear();
	protocol_.response_strings(buffer, msg);
	return WorkerStatus::WRITE;
}

// ====================================

template<class PROTOCOL, class DB_ADAPTER>
std::string KeyValueWorker<PROTOCOL, DB_ADAPTER>::db_info_(const std::nullptr_t *){
	return "Mock Adapter Info";
}

template<class PROTOCOL, class DB_ADAPTER>
std::string KeyValueWorker<PROTOCOL, DB_ADAPTER>::db_get_(const std::nullptr_t *, const StringRef &){
	return "value";
}

template<class PROTOCOL, class DB_ADAPTER>
std::vector<StringRef> KeyValueWorker<PROTOCOL, DB_ADAPTER>::db_getall_(const std::nullptr_t *, const StringRef &){
	return {
		"key1", "value1",
		"key2", "value2",
		"key3", "value3",
		"key4", "value4"
	};
}

} // namespace worker
} // namespace

