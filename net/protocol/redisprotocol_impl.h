namespace net{
namespace protocol{

template<class CONNECTION>
void RedisProtocol::response_empty(CONNECTION &buffer){
	buffer.push("$-1");
	buffer.push(ENDLN);
}

template<class CONNECTION>
void RedisProtocol::response_string(CONNECTION &buffer, const StringRef &msg){
	buffer.push(DOLLAR);
	buffer.push(std::to_string(msg.size()));
	buffer.push(ENDLN);

	buffer.push(msg);
	buffer.push(ENDLN);
}

template<class CONNECTION, class CONTAINER>
void RedisProtocol::response_strings(CONNECTION &buffer, const CONTAINER &list){
	buffer.push(STAR);
	buffer.push(std::to_string(list.size()));
	buffer.push(ENDLN);

	for(const auto &msg : list)
		response_string(buffer, msg);
}

template<class CONNECTION>
void RedisProtocol::response_error(CONNECTION &buffer, const StringRef &msg){
	buffer.push("-ERR ");
	buffer.push(msg);
	buffer.push(ENDLN);
}


} // namespace protocol
} // namespace

