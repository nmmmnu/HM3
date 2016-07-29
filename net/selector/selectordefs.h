#ifndef _NET_STATUS_H
#define _NET_STATUS_H

namespace net{
namespace selector{

enum class WaitStatus{ OK, NONE, ERROR };

enum class FDEvent{ READ, WRITE };

enum class FDStatus{ NONE, READ, WRITE, ERROR, STOP };

struct FDResult{
	int		fd;
	FDStatus	status;

	constexpr FDResult(int const fd, FDStatus const status) : fd(fd), status(status) {}
	constexpr FDResult(FDStatus const status) : FDResult(-1, status){}
};

} // selector
} // namespace

#endif

