#ifndef _KEY_VALUE_WORKER_H
#define _KEY_VALUE_WORKER_H

#include "workerdefs.h"

#include "stringref.h"

#include "iobuffer.h"

#include "stringref.h"

#include <vector>

#include <array>

namespace net{
namespace worker{

struct KeyValueWorkerStrings{
public:
	template <size_t N>
	using Strings	= std::array<StringRef, N>;

	using Strings2	= Strings<2>;
	using Strings4	= Strings<4>;

public:
	static constexpr Strings2 EXIT		{{	"EXIT",		"exit"		}};
	static constexpr Strings2 SHUTDOWN	{{	"SHUTDOWN",	"shutdown"	}};
	static constexpr Strings2 INFO		{{	"INFO",		"info"		}};
	static constexpr Strings4 REFRESH	{{	"SAVE",		"save",
							"BGSAVE",	"bgsave"	}};
	static constexpr Strings2 GET		{{	"GET",		"get"		}};
	static constexpr Strings2 GETALL	{{	"HGETALL",	"hgetall"	}};

public:
	static constexpr bool cmp__(const StringRef &cmd, const Strings4 &m){
		return cmd == m[0] || cmd == m[1] || cmd == m[2] || cmd == m[3];
	}

	static constexpr bool cmp__(const StringRef &cmd, const Strings2 &m){
		return cmd == m[0] || cmd == m[1];
	}
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
