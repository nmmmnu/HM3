#ifndef _IPAIR_H
#define _IPAIR_H

#include <stdio.h>	// FILE
#include <string.h>	// strcmp



class IPair{
public:
	virtual ~IPair(){};

	virtual const char *getKey() const = 0;

	inline int cmp(const char *key) const;
	inline int cmp(const IPair *pair) const;

	virtual bool valid() const = 0;

	virtual size_t getSize() const = 0;

	virtual void print() const = 0;

	virtual bool saveToFile(FILE *F) const = 0;
};



// ===== INLINES =====

inline int IPair::cmp(const char *key) const{
	return key == NULL ? -1 : strcmp(getKey(), key);
}

inline int IPair::cmp(const IPair *pair) const{
	return cmp(pair->getKey());
}

#endif

