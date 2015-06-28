#ifndef _IPAIR_H
#define _IPAIR_H

#include <stdlib.h>	// size_t
#include <stdint.h>
#include <stdio.h>

class IPair{
public:
	static const uint16_t MAX_KEY_SIZE = 0xffff;
	static const uint32_t MAX_VAL_SIZE = 0xffffffff;

public:
	IPair(const void *blob, bool ownership) :
		_blob((const char*) blob), _ownership(ownership){};

	IPair(const void *blob) :
		IPair(blob, false){};

	IPair(IPair && other);

	virtual ~IPair();

	void destroy();

	// ==============================

	virtual const char *getKey() const = 0;
	virtual const char *getVal() const = 0;

	virtual int cmp(const char *key) const = 0;
	virtual bool valid() const = 0;

	virtual size_t getSize() const = 0;

	virtual void print() const;

	// ==============================

	inline explicit operator bool() const;

	inline const void *releaseBlob();

	inline int cmp2(const IPair &pair) const;
	inline bool valid2(const IPair &pair) const;

	inline const void *getBlob() const;

protected:
	inline void setBlob(const void *blob, bool ownership = false);

private:
	const char *_blob;
	bool _ownership;
};

// ==============================

inline IPair::operator bool() const{
	return getBlob();
}

inline const void *IPair::releaseBlob(){
	_ownership = false;
	return _blob;
}

inline int IPair::cmp2(const IPair &pair) const{
	return cmp(pair.getKey());
}

inline bool IPair::valid2(const IPair &pair) const{
	return valid();
}

// ==============================

inline void IPair::setBlob(const void *blob, bool ownership){
	_blob = (const char *) blob;
	_ownership = ownership;
}

inline const void *IPair::getBlob() const{
	return _blob;
}

#endif

