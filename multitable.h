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
	virtual Pair _get(const StringRef &key) const final;

	count_type _getCount() const final{
		return 0;
	};

	size_t _getSize() const final{
		return 0;
	}

	virtual std::unique_ptr<IIterator> _getIterator() const final;

private:
	IListCollection & _collection;
};

#endif

