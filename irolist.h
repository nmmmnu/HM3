#ifndef _IROLIST_H
#define _IROLIST_H

#include "iiterator.h"
#include "icountable.h"
#include "iversion.h"

#include <memory>

class IROList : virtual public IVersion, virtual public ICountable{
public:
	Pair get(const char *key) const;
	Pair operator[](const char *key) const;
	bool exists(const char *key) const;

	size_t getSize() const;

	std::unique_ptr<IIterator> getIterator() const;

	void print();

private:
	virtual Pair _get(const char *key) const = 0;
	virtual size_t _getSize() const = 0;

	virtual std::unique_ptr<IIterator> _getIterator() const = 0;
};

// ==============================


inline Pair IROList::get(const char *key) const{
	return _get(key);
}

inline Pair IROList::operator[](const char *key) const{
	return _get(key);
}

inline bool IROList::exists(const char *key) const{
	return (bool) _get(key);
}

inline size_t IROList::getSize() const{
	return _getSize();
}

inline std::unique_ptr<IIterator> IROList::getIterator() const{
	return _getIterator();
}

inline void IROList::print(){
	this->getIterator()->print();
}

#endif

