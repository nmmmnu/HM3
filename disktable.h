#ifndef _DISK_TABLE_H
#define _DISK_TABLE_H

#include "iarray.h"
#include "irolist.h"

class DiskTable : virtual public IArray{
public:
	DiskTable(const char *filename);
	virtual ~DiskTable() override;

	bool open();
	void close();

	virtual uint64_t getCount() const override{
		return _datacount;
	}

	virtual std_optional<const Pair> getAt(uint64_t index) const override;

	virtual size_t getSize() const override;

public:
	       static bool create(const char *filename, IIterator &it, uint64_t datacount);
	inline static bool create(const char *filename, IROList &list);

private:
	const char	*_filename;
	FILE		*_f;
	off_t		_size		= 0;
	const void	*_mem		= NULL;

	uint64_t	_datacount	= 0;

private:
	uint64_t _getCount() const;

	constexpr static size_t __sizeofHeader();

	static bool _writeIteratorToFile(IIterator &it, uint64_t datacount, FILE *F);

};

// ==============================

inline bool DiskTable::create(const char *filename, IROList &list){
	return create(filename, list.getIterator(), list.getCount());
}

#endif
