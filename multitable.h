#ifndef _DIR_TABLE_H
#define _DIR_TABLE_H

#include "itable.h"
#include "ilistcollection.h"

class MultiTableIterator;

class MultiTable : virtual public ITable{
friend class MultiTableIterator;
public:
	MultiTable(IListCollection & collection) : _collection(collection){};

private:
	virtual Pair _get(const char *key) const override;

	virtual uint64_t _getCount() const override{
		return 0;
	};

	virtual size_t _getSize() const override{
		return 0;
	}

	virtual std::unique_ptr<IIterator> _getIterator() const override;

private:
	IListCollection & _collection;
};

#endif

