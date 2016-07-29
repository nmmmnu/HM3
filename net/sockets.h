#ifndef _NET_SOCKETS_H
#define _NET_SOCKETS_H

#include <cstdint>

namespace net{

constexpr int SOCKET_NONBLOCK	= 0x01;
constexpr int SOCKET_TCPNODELAY	= 0x02;

// ===========================

constexpr int SOCKET_DEFAULTOPT_TCP	= SOCKET_NONBLOCK & SOCKET_TCPNODELAY;
constexpr int SOCKET_DEFAULTOPT_UNIX	= SOCKET_NONBLOCK;

// ===========================

constexpr int SOCKET_ERROR_CREATE	= -1;
constexpr int SOCKET_ERROR_OPTIONS	= -2;
constexpr int SOCKET_ERROR_NONBLOCK	= -3;
constexpr int SOCKET_ERROR_NODELAY	= -4;
constexpr int SOCKET_ERROR_BIND		= -5;
constexpr int SOCKET_ERROR_BACKLOG	= -6;
constexpr int SOCKET_NAME_SIZE		= -7;

// ===========================

struct SOCKET_TCP{};
struct SOCKET_UNIX{};

extern SOCKET_TCP	socket_tcp;
extern SOCKET_UNIX	socket_unix;

// ===========================

int socket_create(const SOCKET_TCP,  const char *ip, uint16_t port, uint16_t backlog = 0, int options = SOCKET_DEFAULTOPT_TCP) noexcept;
int socket_create(const SOCKET_UNIX, const char *path,              uint16_t backlog = 0, int options = SOCKET_DEFAULTOPT_UNIX) noexcept;

bool socket_makeNonBlocking(int fd) noexcept;
bool socket_makeTCPNoDelay(int fd) noexcept;

int socket_accept(int fd) noexcept;

void socket_close(int fd) noexcept;

bool socket_check_eagain() noexcept;

}

#endif

