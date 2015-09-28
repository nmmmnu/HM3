#ifndef _IVERSION_H
#define _IVERSION_H

class IVersion{
public:
	uint64_t getVersion() const noexcept{
		return _version;
	}

protected:
	void     incVersion() noexcept;

private:
	uint64_t _version = 0;

};

// ==============================

inline void IVersion::incVersion() noexcept{
	++_version;
}

#endif

