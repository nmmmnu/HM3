#include "redisprotocol.h"

#include <iostream>


namespace net{
namespace protocol{


enum class RedisProtocol::Error{
	NO_STAR,
	NO_DOLLAR,
	PARAM_COUNT,
	PARAM_SIZE,
	NO_CRLF
};


void RedisProtocol::print() const{
	for(const auto &item : params_ )
		std::cout << item << std::endl;
}


auto RedisProtocol::operator()(const StringRef &src) -> Status{
	// this doing more harm than relps.
//	if (src.size() < 8)	// 4 bytes - "*1\r\n$1\r\n"
//		return Status::BUFFER_NOT_READ;

	size_t pos = 0;

	if (src[pos] != STAR)
		return err_(Error::NO_STAR);

	++pos;

	if (pos + 1 > src.size())
		// at least one character for parameter count
		return Status::BUFFER_NOT_READ;

	size_t const paramsCount = size_t( readInt_(src, pos) );

	params_.clear();

	if (paramsCount == 0 || paramsCount > MAX_PARAMS)
		return err_(Error::PARAM_COUNT);

	{
		Status stat = readLn_(src, pos);
		if ( stat != Status::OK )
			return stat;
	}

	for(size_t i = 0; i < paramsCount; ++i)
		if ( const auto p = readParam_(src, pos) )
			params_.push_back( p.get() );
		else
			return p.code();

	return Status::OK;
}


int RedisProtocol::readInt_(const StringRef &src, size_t &pos){
	char buff[INT_BUFFER_SIZE + 1];

	size_t bpos = 0;

	while(pos < src.size() && bpos < INT_BUFFER_SIZE){
		char const c = src[pos];
		if (c != '-' && c != '+' && (c < '0' || c > '9') )
			break;

		buff[bpos] = c;

		++pos;
		++bpos;
	}

	buff[bpos] = '\0';

	return atoi(buff);
}


auto RedisProtocol::readLn_(const StringRef &src, size_t &pos) -> Status{
	if (pos + 1 > src.size())
		return Status::BUFFER_NOT_READ;

	if (src[pos] == '\r' && src[pos + 1] == '\n'){
		pos += 2;
		return Status::OK;
	}

	return err_(Error::NO_CRLF);
}


auto RedisProtocol::readParam_(const StringRef &src, size_t &pos) -> CheckedStringref{
	if (pos + 1 > src.size())
		return Status::BUFFER_NOT_READ;

	if (src[pos] != DOLLAR)
		return err_(Error::NO_DOLLAR);

	++pos;

	if (pos + 1 > src.size())
		return Status::BUFFER_NOT_READ;

	const size_t size = size_t( readInt_(src, pos) );
	if (size <= 0 || size > MAX_PARAM_SIZE)
		return err_(Error::PARAM_SIZE);

	{
		Status stat = readLn_(src, pos);
		if ( stat != Status::OK )
			return stat;
	}

	const auto pos_save = pos;

	// store param must be here, but is optimized at the end.

	pos += size;

	// check if all data is there.
	if (pos > src.size())
		return Status::BUFFER_NOT_READ;

	{
		Status stat = readLn_(src, pos);
		if ( stat != Status::OK )
			return stat;
	}

	return StringRef( & src[pos_save], size);
}


} // namespace protocol
} // namespace

