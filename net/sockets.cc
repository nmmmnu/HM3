#include "sockets.h"

#include <sys/socket.h>		// EAGAIN
#include <arpa/inet.h>		// AF_INET
#include <sys/un.h>		// AF_UNIX
#include <netinet/tcp.h>	// TCP_NODELAY
#include <netinet/in.h>		// IPPROTO_TCP for FreeBSD 
#include <fcntl.h>		// fcntl
#include <string.h>		// strlen
#include <unistd.h>		// close

#include <errno.h>		// errno

namespace net{
	
inline bool socket_setOption_(int const fd, int const proto, int const opt, int const val = 1);
inline int socket_error_(int const fd, int const error);
template <class SOCKADDR>
int socket_server_(int const fd, SOCKADDR &address, uint16_t const backlog);

// ===========================

bool socket_check_eagain() noexcept{
	return errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK;
}

bool socket_makeNonBlocking(int const fd) noexcept{
	return fcntl(fd, F_SETFL, O_NONBLOCK) >= 0;
}


bool socket_makeReuseAddr_(int const fd) noexcept{
	return socket_setOption_(fd, SOL_SOCKET, SO_REUSEADDR);
}

bool socket_makeTCPNoDelay(int const fd) noexcept{
	return socket_setOption_(fd, IPPROTO_TCP, TCP_NODELAY);
}

bool socket_makeKeepAlive(int const fd) noexcept{
	return socket_setOption_(fd, SOL_SOCKET, SO_KEEPALIVE);
}

void socket_close(int const fd) noexcept{
	::close(fd);
}

int socket_accept(int const fd) noexcept{
	return ::accept(fd, nullptr, nullptr);
}

// ===========================

int socket_create(const SOCKET_TCP,  const char *ip, uint16_t const port, uint16_t const backlog, options_type const options) noexcept{
	int fd = socket(AF_INET , SOCK_STREAM , 0);

	if(fd < 0)
		return SOCKET_ERROR_CREATE;

	if (options & SOCKET_REUSEADDR)
	if (! socket_makeReuseAddr_(fd))
		socket_error_(fd, SOCKET_ERROR_OPTIONS);

	if (options & SOCKET_NONBLOCK)
	if (! socket_makeNonBlocking(fd) )
		socket_error_(fd, SOCKET_ERROR_NONBLOCK);

	if (options & SOCKET_TCPNODELAY)
	if (! socket_makeTCPNoDelay(fd) )
		socket_error_(fd, SOCKET_ERROR_NODELAY);

	if (options & SOCKET_KEEPALIVE)
	if (! socket_makeKeepAlive(fd) )
		socket_error_(fd, SOCKET_ERROR_KEEPALIVE);

	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	return socket_server_(fd, address, backlog);
}

int socket_create(const SOCKET_UNIX, const char *path, uint16_t const backlog, options_type const options) noexcept{
	if (sizeof sockaddr_un::sun_path < strlen(path))
		return SOCKET_NAME_SIZE;


	int fd = socket(AF_UNIX , SOCK_STREAM , 0);

	if(fd < 0)
		return SOCKET_ERROR_CREATE;

	if (options & SOCKET_NONBLOCK)
	if (! socket_makeNonBlocking(fd) )
		socket_error_(fd, SOCKET_ERROR_NONBLOCK);

	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, path);

	unlink(address.sun_path);

	return socket_server_(fd, address, backlog);
}

// ===========================

inline bool socket_setOption_(int const fd, int const proto, int const opt, int const val){
	return setsockopt(fd, proto, opt, & val, sizeof val) >= 0;
}

template <class SOCKADDR>
int socket_server_(int const fd, SOCKADDR &address, uint16_t const backlog){
	if (bind(fd, (struct sockaddr *) & address, sizeof address) < 0){
		::close(fd);
		return SOCKET_ERROR_BIND;
	}

	if (listen(fd, backlog ? backlog : SOMAXCONN) < 0){
		::close(fd);
		return SOCKET_ERROR_BACKLOG;
	}

	return fd;
}

inline int socket_error_(int const fd, int const error){
	::close(fd);
	return error;
}

} // namespace

