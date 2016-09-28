#ifndef _NET_SOCKETS_H
#define _NET_SOCKETS_H

#include <cstdint>

namespace net{

using options_type = uint8_t;

constexpr options_type SOCKET_NONBLOCK		= 1 << 0;
constexpr options_type SOCKET_REUSEADDR		= 1 << 1;
constexpr options_type SOCKET_TCPNODELAY	= 1 << 2;
constexpr options_type SOCKET_KEEPALIVE		= 1 << 3;

// ===========================

constexpr options_type SOCKET_DEFAULTOPT_TCP	= SOCKET_NONBLOCK | SOCKET_REUSEADDR | SOCKET_TCPNODELAY;
constexpr options_type SOCKET_DEFAULTOPT_UNIX	= SOCKET_NONBLOCK;

// ===========================

constexpr int SOCKET_ERROR_CREATE	= -1;
constexpr int SOCKET_ERROR_OPTIONS	= -2;
constexpr int SOCKET_ERROR_NONBLOCK	= -3;
constexpr int SOCKET_ERROR_NODELAY	= -4;
constexpr int SOCKET_ERROR_KEEPALIVE	= -5;
constexpr int SOCKET_ERROR_BIND		= -6;
constexpr int SOCKET_ERROR_BACKLOG	= -7;
constexpr int SOCKET_NAME_SIZE		= -8;

// ===========================

struct SOCKET_TCP{  constexpr SOCKET_TCP(){}  };
struct SOCKET_UNIX{ constexpr SOCKET_UNIX(){} };

constexpr SOCKET_TCP	socket_tcp;
constexpr SOCKET_UNIX	socket_unix;

// ===========================

int socket_create(const SOCKET_TCP,  const char *ip, uint16_t port, uint16_t backlog = 0, options_type options = SOCKET_DEFAULTOPT_TCP) noexcept;
int socket_create(const SOCKET_UNIX, const char *path,              uint16_t backlog = 0, options_type options = SOCKET_DEFAULTOPT_UNIX) noexcept;

bool socket_makeNonBlocking(int fd) noexcept;
bool socket_makeReuseAddr(int fd) noexcept;
bool socket_makeTCPNoDelay(int fd) noexcept;
bool socket_makeKeepAlive(int fd) noexcept;

int socket_accept(int fd) noexcept;

void socket_close(int fd) noexcept;

bool socket_check_eagain() noexcept;

}

#endif

