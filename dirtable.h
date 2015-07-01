#ifndef _DIR_TABLE_H
#define _DIR_TABLE_H

#include "irolist.h"
#include "disktable.h"

#include <vector>
#include <memory>

class DirTable : virtual public IROList{
public:
	virtual ~DirTable() override{
		close();
	}

	bool open(const char *path);
	void close();

	virtual uint64_t getCount() const override{
		return 0;
	};

	virtual size_t getSize() const override{
		return 0;
	}

	virtual const Pair *get(const char *key) const override;

	virtual bool rewind(const char *key = nullptr) override;

protected:
	virtual const Pair *_next() override;

private:
	DiskTable	*_files		= nullptr;
	size_t		_filesCount	= 0;
};

#endif
