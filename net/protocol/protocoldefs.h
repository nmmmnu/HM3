#ifndef _PROTOCOL_DEFS_H
#define _PROTOCOL_DEFS_H

namespace net{
namespace protocol{

	enum class ProtocolStatus{
		OK,
		BUFFER_NOT_READ,
		ERROR
	};

} // namespace protocol
} // namespace

#endif

