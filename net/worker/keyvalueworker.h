#ifndef _KEY_VALUE_WORKER_H
#define _KEY_VALUE_WORKER_H

#include "workerdefs.h"

#include "stringref.h"

#include "iobuffer.h"

#include "stringref.h"

#include <vector>


namespace net{
namespace worker{

struct KeyValueWorkerStrings{
	static constexpr StringRef EXIT		= "EXIT";
	static constexpr StringRef EXIT2	= "exit";

	static constexpr StringRef SHUTDOWN	= "SHUTDOWN";
	static constexpr StringRef SHUTDOWN2	= "shutdown";

	static constexpr StringRef INFO		= "INFO";
	static constexpr StringRef INFO2	= "info";

	static constexpr StringRef RELOAD	= "RELOAD";
	static constexpr StringRef RELOAD2	= "reload";

	static constexpr StringRef GET		= "GET";
	static constexpr StringRef GET2		= "get";

	static constexpr StringRef GETALL	= "HGETALL";
	static constexpr StringRef GETALL2	= "hgetall";

};

template<class PROTOCOL, class DB_ADAPTER>
class KeyValueWorker : private KeyValueWorkerStrings{
public:
	KeyValueWorker(DB_ADAPTER &db) : db_(db){}

	template<class CONNECTION>
	WorkerStatus operator()(CONNECTION &buffer);

private:
	template<class CONNECTION>
	WorkerStatus process_request_(CONNECTION &buffer);

	enum class ResponseType{
		ERROR,
		STRING,
		MULTI
	};

	template<class CONNECTION>
	WorkerStatus sendResponseError_(CONNECTION &buffer, const StringRef &msg);

	template<class CONNECTION>
	WorkerStatus sendResponseString_(CONNECTION &buffer, const StringRef &msg);

	template<class CONNECTION, class CONTAINER>
	WorkerStatus sendResponseMulti_(CONNECTION &buffer, const CONTAINER &msg);

private:
	PROTOCOL	protocol_;
	DB_ADAPTER	&db_;
};


} // namespace worker
} // namespace


// ==================================

#include "keyvalueworker_impl.h"

#endif
