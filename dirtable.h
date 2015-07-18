#ifndef _DIR_TABLE_H
#define _DIR_TABLE_H

#include "irolist.h"
#include "disktable.h"

class DirTable : virtual public IROList{
public:
	virtual ~DirTable() override{
		close();
	}

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

	virtual void _rewind(const char *key = nullptr) override;
	virtual const void *_next() override;

private:
	DiskTable	*_files		= nullptr;
	size_t		_filesCount	= 0;
};

#endif
