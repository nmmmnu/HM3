#ifndef _ITable_H
#define _ITable_H

#include "iiterator.h"
#include "icountable.h"
#include "iversion.h"

#include "stringref.h"

#include <memory>

class ITable : virtual public IVersion, public ICountable<ITable>{
friend class ICountable<ITable>;

public:
	Pair get(const StringRef &key) const;
	Pair operator[](const StringRef &key) const;
	bool exists(const StringRef &key) const;

	size_t getSize() const;

	std::unique_ptr<IIterator> getIterator() const;

	void print() const;

private:
	virtual Pair _get(const StringRef &key) const = 0;
	virtual size_t _getSize() const = 0;
	virtual count_type _getCount() const = 0;

	virtual std::unique_ptr<IIterator> _getIterator() const = 0;
};

// ==============================


inline Pair ITable::get(const StringRef &key) const{
	return _get(key);
}

inline Pair ITable::operator[](const StringRef &key) const{
	return _get(key);
}

inline bool ITable::exists(const StringRef &key) const{
	return (bool) _get(key);
}

inline size_t ITable::getSize() const{
	return _getSize();
}

inline std::unique_ptr<IIterator> ITable::getIterator() const{
	return _getIterator();
}

inline void ITable::print() const{
	this->getIterator()->print();
}

#endif

