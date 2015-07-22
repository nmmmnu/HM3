#ifndef _DIR_TABLE_H
#define _DIR_TABLE_H

#include "irolist.h"
#include "disktable.h"

#include <deque>

class DirTableIterator;

class DirTable : virtual public IROList{
friend class DirTableIterator;
public:
	bool open(const char *path);
	void close();

protected:
	virtual Pair _get(const char *key) const override;

	virtual uint64_t _getCount() const override{
		return 0;
	};

	virtual size_t _getSize() const override{
		return 0;
	}

	virtual std::unique_ptr<IIterator> _getIterator() const override;

private:
	std::deque<DiskTable> _files;
};

#endif
