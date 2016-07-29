namespace net{
namespace protocol{

template<class CONNECTION>
void RedisProtocol::response_empty(CONNECTION &buffer){
	buffer.push("$-1\r\n");
}

template<class CONNECTION>
void RedisProtocol::response_string(CONNECTION &buffer, const StringRef &msg){
	response_count_(buffer, DOLLAR, msg.size());

	buffer.push(msg);
	buffer.push("\r\n");
}

template<class CONNECTION, class CONTAINER>
void RedisProtocol::response_strings(CONNECTION &buffer, const CONTAINER &list){
	response_count_(buffer, STAR, list.size());

	for(const auto &msg : list)
		response_string(buffer, msg);
}

template<class CONNECTION>
void RedisProtocol::response_error(CONNECTION &buffer, const StringRef &msg){
	buffer.push("-ERR ");
	buffer.push(msg);
	buffer.push("\r\n");
}

template<class CONNECTION>
void RedisProtocol::response_count_(CONNECTION &buffer, char const c, size_t const count){
	buffer.push(c);

	// lets be super safe here.
	int const ss = snprintf(nullptr, 0, "%zu", count);
	if (ss <= INT_BUFFER_SIZE){
		char s[INT_BUFFER_SIZE + 1];
		sprintf(s, "%zu", count);
		buffer.push(s);
	}else{
		buffer.push("ERROR");
	}

	buffer.push("\r\n");
}

} // namespace protocol
} // namespace

