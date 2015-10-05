#ifndef _IVERSION_H
#define _IVERSION_H

#include <cstdint>

class IVersion{
protected:
	~IVersion() = default;

public:
	uint64_t getVersion() const noexcept{
		return _version;
	}

protected:
	void     incVersion() noexcept{
		++_version;
	}
private:
	uint64_t _version = 0;

};

#endif

