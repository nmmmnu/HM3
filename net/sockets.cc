#include "sockets.h"

#include <sys/socket.h>		// EAGAIN
#include <arpa/inet.h>		// AF_INET
#include <sys/un.h>		// AF_UNIX
#include <netinet/tcp.h>	// TCP_NODELAY
#include <fcntl.h>		// fcntl
#include <netinet/in.h>		// IPPROTO_TCP for FreeBSD 
#include <string.h>		// strlen
#include <unistd.h>		// close

#include <errno.h>		// errno

namespace net{

SOCKET_TCP	socket_tcp;
SOCKET_UNIX	socket_unix;



bool socket_check_eagain() noexcept{
	return errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK;
}

bool socket_makeNonBlocking(int const fd) noexcept{
	if (fd >= 0)
		return fcntl(fd, F_SETFL, O_NONBLOCK) >= 0;

	return false;
}

bool socket_makeTCPNoDelay(int const fd) noexcept{
	int const opt = 1;
	return (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, & opt, sizeof opt) >= 0);
}

void socket_close(int const fd) noexcept{
	if (fd >= 0)
		::close(fd);
}

int socket_accept(int const fd) noexcept{
	if (fd >= 0)
		return ::accept(fd, NULL, NULL);

	return -1;
}

int socket_create(const SOCKET_TCP,  const char *ip, uint16_t const port, uint16_t const backlog, int const options) noexcept{
	int fd = socket(AF_INET , SOCK_STREAM , 0);

	if(fd < 0)
		return SOCKET_ERROR_CREATE;

	int const opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, & opt, sizeof opt) < 0){
		::close(fd);
		return SOCKET_ERROR_OPTIONS;
	}

	if (options & SOCKET_NONBLOCK)
	if (! socket_makeNonBlocking(fd) ){
		::close(fd);
		return SOCKET_ERROR_NONBLOCK;
	}

	if (options & SOCKET_TCPNODELAY)
	if (! socket_makeTCPNoDelay(fd) ){
		::close(fd);
		return SOCKET_ERROR_NODELAY;
	}

	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

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

int socket_create(const SOCKET_UNIX, const char *path, uint16_t const backlog, int const options) noexcept{
	struct sockaddr_un address;

	if (sizeof address.sun_path < strlen(path))
		return SOCKET_NAME_SIZE;


	int fd = socket(AF_UNIX , SOCK_STREAM , 0);

	if(fd < 0)
		return SOCKET_ERROR_CREATE;

	if (options & SOCKET_NONBLOCK)
	if (! socket_makeNonBlocking(fd) ){
		::close(fd);
		return SOCKET_ERROR_NONBLOCK;
	}

	//struct sockaddr_in address
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, path);

	unlink(address.sun_path);

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


} // namespace

