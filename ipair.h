#ifndef _IPAIR_H
#define _IPAIR_H

class IPair{
public:
	virtual ~IPair(){};
	
	virtual const char *getKey() const = 0;
	virtual const char *getValue() const = 0;
	
	virtual int cmp(const char *key) const = 0;
	
	virtual bool valid() const = 0;
	
	inline int cmp(const IPair *pair) const{
		return cmp(getKey());
	}
};

#endif
