#ifndef _REDIS_PROTOCOL_H
#define _REDIS_PROTOCOL_H

#include "stringref.h"
#include "checkedvalue.h"

#include "protocoldefs.h"

#include <vector>

#include <sstream>

namespace net{
namespace protocol{

class RedisProtocol{
public:
	using Status = ProtocolStatus;

	using StringVector = std::vector<StringRef>;

public:
	RedisProtocol(){
		params_.reserve(MAX_PARAMS);
	}

public:
	Status operator()(const StringRef &src);

	const StringVector &getParams() const{
		return params_;
	}

	void print() const;

public:
	template<class CONNECTION>
	void response_empty(CONNECTION &buffer);

	template<class CONNECTION>
	void response_string(CONNECTION &buffer, const StringRef &msg);

	template<class CONNECTION, class CONTAINER>
	void response_strings(CONNECTION &buffer, const CONTAINER &list);

	template<class CONNECTION>
	void response_error(CONNECTION &buffer, const StringRef &msg);

private:
	template<class CONNECTION>
	void response_count_(CONNECTION &buffer, char c, size_t count);

private:
	static int readInt_(const StringRef &src, size_t &pos);
	static Status readLn_(const StringRef &src, size_t &pos);

	using CheckedStringref = CheckedValue<StringRef, Status, Status::OK>;

	static CheckedStringref readParam_(const StringRef &src, size_t &pos);

private:
	enum class Error;

	constexpr static Status err_(const Error &){
		return Status::ERROR;
	}

private:
	constexpr static char	STAR		= '*';
	constexpr static char	DOLLAR		= '$';

	constexpr static size_t	INT_BUFFER_SIZE	= 8;	// to be able to store MAX_PARAM_SIZE as string.
	constexpr static size_t	MAX_PARAMS	= 4;	// setex name 100 hello
	constexpr static size_t	MAX_PARAM_SIZE	= 16 * 1024;

private:
	StringVector	params_;
};


} // namespace protocol
} // namespace

#include "redisprotocol_impl.h"

#endif

