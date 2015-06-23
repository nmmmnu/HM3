#ifndef _IARRAY_H
#define _IARRAY_H

#include "pair.h"
#include "iiterator.h"
#include "icountable.h"

class IArray : virtual public IIterator, virtual public ICountable{
public:
	static const uint8_t LINEAR_SEARCH	= 0;
	static const uint8_t BIN_SEARCH		= 1;

private:
	static const uint8_t DEFAULT_SEARCH	= BIN_SEARCH;

public:
	virtual ~IArray(){};

	virtual const Pair *getAt(uint64_t index) const = 0;

	inline const Pair *operator[](uint64_t index) const;

	inline int lookup(const char *key, uint64_t *index = NULL) const;

	inline void setSearchMethod(uint8_t searchMethod);

public:
	virtual bool rewind(const char *key = NULL) override;
	virtual const Pair *next() override;

private:
	uint8_t		_searchMethod = DEFAULT_SEARCH;

	uint64_t	_itPos = 0;

private:
	int _lookupBinSearch(const char *key, uint64_t *index) const;
	int _lookupLinearSearch(const char *key, uint64_t *index) const;

};

// ==============================

inline const Pair *IArray::operator[](uint64_t index) const{
	return getAt(index);
}

inline void IArray::setSearchMethod(uint8_t searchMethod){
	_searchMethod = searchMethod;
}

inline int IArray::lookup(const char *key, uint64_t *index) const{
	switch(_searchMethod){
	case LINEAR_SEARCH:	return _lookupLinearSearch(key, index);
	default:
	case BIN_SEARCH:	return _lookupBinSearch(key, index);
	}
}

#endif

