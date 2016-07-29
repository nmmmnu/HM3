#ifndef _KEY_VALUE_WORKER_H
#define _KEY_VALUE_WORKER_H

#include "workerdefs.h"

#include "stringref.h"

#include <vector>


namespace net{
namespace worker{

template<class PROTOCOL, class DB_ADAPTER = std::nullptr_t>
class KeyValueWorker{
public:
	KeyValueWorker(DB_ADAPTER *db = nullptr) : db_(db){}

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
	static constexpr const char *EXIT	= "EXIT";
	static constexpr const char *EXIT2	= "exit";

	static constexpr const char *SHUTDOWN	= "SHUTDOWN";
	static constexpr const char *SHUTDOWN2	= "shutdown";

	static constexpr const char *RELOAD	= "RELOAD";
	static constexpr const char *RELOAD2	= "reload";

	static constexpr const char *GET	= "GET";
	static constexpr const char *GET2	= "get";

	static constexpr const char *GETALL	= "HGETALL";
	static constexpr const char *GETALL2	= "hgetall";

private:
	// Mock commands
	static StringRef db_get_(const std::nullptr_t *, const StringRef &key);
	static std::vector<StringRef> db_getall_(const std::nullptr_t *, const StringRef &key);

	// Actual commands
	template<class DB_ADAPTER_>
	static StringRef db_get_(const DB_ADAPTER_ *db, const StringRef &key){
		if (! db)
			return {};

		return db->get(key);
	}

	template<class DB_ADAPTER_>
	static std::vector<std::string> db_getall_(const DB_ADAPTER_ *db, const StringRef &key){
		if (! db)
			return {};

		return db->getall(key);
	}

private:
	PROTOCOL	protocol_;
	DB_ADAPTER	*db_;
};


} // namespace worker
} // namespace


// ==================================

#include "keyvalueworker_impl.h"

#endif
