#ifndef _IVERSION_H
#define _IVERSION_H

class IVersion{
public:
	virtual ~IVersion(){};
	
	uint64_t getVersion() const;

protected:
	void     incVersion();

private:
	uint64_t _version = 0;

};

// ==============================

inline uint64_t IVersion::getVersion() const{
	return _version;
}

inline void IVersion::incVersion(){
	++_version;
}

#endif

