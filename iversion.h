#ifndef _IVERSION_H
#define _IVERSION_H

#include <cstdint>

class IVersion{
protected:
	~IVersion() = default;

public:
	typedef uint32_t version_type;

public:
	version_type getVersion() const noexcept{
		return _version;
	}

protected:
	void     incVersion() noexcept{
		++_version;
	}
private:
	version_type _version = 0;

};

#endif

